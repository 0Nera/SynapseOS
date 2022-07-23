/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/


#include <kernel.h>

// Defone some address calculation macro
#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >>12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

page_directory *kernel_page_dir; // Pointer (physical) to kernel page dircetory structure

extern void enable_paging(physical_addres_t page);

uint32_t kv2p(void *a) {
    return (uint32_t)a - 0xC0000000;
}

void * virtual2phys(page_directory * dir, void * virtual_addr_t) {
    uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr_t), page_tbl_idx = PAGETBL_INDEX(virtual_addr_t), page_frame_offset = PAGEFRAME_INDEX(virtual_addr_t);
    if(!dir->ref_tables[page_dir_idx]) {
        qemu_log("virtual2phys: page dir entry %d does not exist\n", page_tbl_idx);
        return NULL;
    }
    page_table * table = dir->ref_tables[page_dir_idx];
    if(!table->entries[page_tbl_idx]) {
        qemu_log("virtual2phys: page table entry %d does not exist\n", page_tbl_idx);
        return NULL;
    }
    uint32_t t = table->entries[page_tbl_idx];
    t = (t << 12) + page_frame_offset;
    return (void*)t;
}

bool vmm_alloc_page(virtual_addr_t vaddr) {
    physical_addres_t paddr = pmm_alloc_block();
    if (!paddr) {
        return false;
    }

    vmm_map_page(paddr, vaddr);
    return true;
}


bool vmm_alloc_page_with_userbit(virtual_addr_t vaddr) {
    physical_addres_t paddr = pmm_alloc_block();

    if (!paddr) {
        return false;
    }

    vmm_map_page(paddr, vaddr);
    page_table_entry *pte = GET_PTE(vaddr);
    page_table_entry_add_attrib(pte, I86_PTE_USER);
    return true;
}


void vmm_free_page(virtual_addr_t vaddr) {
    page_table_entry *pte = GET_PTE(vaddr);

    if (!page_table_entry_is_present(*pte)) {
        tty_printf("oh, you try to delete not present page\n");
        return;
    }

    physical_addres_t block = page_table_entry_frame(*pte);
    if (block) {
        pmm_free_block(block);
    }
    page_table_entry_del_attrib(pte, I86_PTE_PRESENT);
}


void vmm_create_kernel_page_dir() {
    kernel_page_dir = (page_directory*) pmm_alloc_block();

    if (kernel_page_dir == (page_directory*)0xFFFFFFFF) {
        tty_printf("Failed to allocate phys memory for kernel page dir\n");
        return;
    }

    page_directory *pd = kernel_page_dir;
    memset(pd, 0, sizeof(page_directory));

    int32_t i;
    for (i = 0; i < PAGE_ENTRIES; i++) {
        page_dir_entry *pde = (page_dir_entry*) &pd->entries[i];
        page_dir_entry_add_attrib(pde, I86_PTE_WRITABLE);
        page_dir_entry_del_attrib(pde, I86_PTE_PRESENT);

        if (i == PAGE_ENTRIES - 1) { // Техника фрактального (рекурсивного) отображения, которая позволяет нам получить доступ к PD и PT
            page_dir_entry_add_attrib(pde, I86_PTE_PRESENT);
            page_dir_entry_set_frame(pde, (physical_addres_t) kernel_page_dir);
        }
    }
}


void vmm_map_page(physical_addres_t paddr, virtual_addr_t vaddr) {
    page_dir_entry *pde = GET_PDE(vaddr);

    if (!page_dir_entry_is_present(*pde)) {                         // Если таблицы страниц нет, создайте ее
        physical_addres_t pt_p = pmm_alloc_block();                   // Это ФИЗИЧЕСКИЙ адрес
        if (pt_p == 0xFFFFFFFF) {
            tty_printf("No free phys memory. Minimum 3 mb\n");
            return;
        }

        page_table *pt_v = (page_table*) vmm_temp_map_page(pt_p);   // Потому что нам нужно писать!
        memset(pt_v, 0, sizeof(page_table));
        page_dir_entry_add_attrib(pde, I86_PDE_PRESENT);
        page_dir_entry_add_attrib(pde, I86_PDE_WRITABLE);
        page_dir_entry_set_frame(pde, pt_p);
    }

    page_table_entry *pte = GET_PTE(vaddr);
    page_table_entry_set_frame(pte, paddr);
    page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
    page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);
    flush_tlb_entry(vaddr);
}


virtual_addr_t vmm_temp_map_page(physical_addres_t paddr) {
    page_table_entry *pte = GET_PTE(TEMP_PAGE_ADDR);

    page_table_entry_set_frame(pte, PAGE_ALIGN_DOWN(paddr));
    page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
    page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);

    asm volatile("invlpg %0" :: "m" (*(uint32_t *) TEMP_PAGE_ADDR) : "memory" );

    return TEMP_PAGE_ADDR;
}


// Переключить каталог страниц, восстанавливает физический адрес
void vmm_switch_page_directory(page_directory *page_dir_phys_addr) {
    asm volatile("mov %0, %%cr3" :: "r"((uint32_t) page_dir_phys_addr));
}


void vmm_init() {
    vmm_create_kernel_page_dir();

    page_table *table1 = (page_table*) pmm_alloc_block();
    page_table *table2 = (page_table*) pmm_alloc_block();

    memset((void*) table1, 0, sizeof(page_table));      // Очистить выделенные таблицы страниц
    memset((void*) table2, 0, sizeof(page_table));

    physical_addres_t frame;                              // Сопоставляет первый МБ с 3 ГБ
    virtual_addr_t virt;
    for (frame = 0x0, virt = 0xC0000000;
         frame < 0x100000;
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table1->entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    // Сопоставляет страницы ядра и страницы физической памяти
    for (frame = KERNEL_START_PADDR, virt = KERNEL_START_VADDR;
         frame < KERNEL_PHYS_MAP_END;
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table2->entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    page_dir_entry *pde1 = (page_dir_entry*) &kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    page_dir_entry_add_attrib(pde1, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde1, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde1, (physical_addres_t) table1);

    page_dir_entry **pde2 = (page_dir_entry**) &kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0xC0100000)];
    page_dir_entry_add_attrib((page_dir_entry*)pde2, I86_PDE_PRESENT);
    page_dir_entry_add_attrib((page_dir_entry*)pde2, I86_PDE_WRITABLE);
    page_dir_entry_set_frame((page_dir_entry*)pde2, (physical_addres_t) table2);

    update_phys_memory_bitmap_addres(KERNEL_END_VADDR);

    enable_paging((physical_addres_t) kernel_page_dir);
    qemu_log("VMM installed");
}


// Add attribute to pte
void page_table_entry_add_attrib(page_table_entry *entry, uint32_t attrib) {
    *entry |= attrib;
}


// Delete attribute to pte
void page_table_entry_del_attrib(page_table_entry *entry, uint32_t attrib) {
    *entry &= ~attrib;
}


// Map pte to physical frame
void page_table_entry_set_frame(page_table_entry *entry, physical_addres_t addr) {
    *entry = (*entry & ~I86_PTE_FRAME) | addr;
}


bool page_table_entry_is_present(page_table_entry entry) {
    return entry & I86_PTE_PRESENT;
}



// Return the address of physical frame which pte refers to
physical_addres_t page_table_entry_frame(page_table_entry entry) {
    return entry & I86_PTE_FRAME;
}


// Add attribute to pde
void page_dir_entry_add_attrib(page_dir_entry *entry, uint32_t attrib) {
    *entry |= attrib;
}


// Delete attribute to pde
void page_dir_entry_del_attrib(page_dir_entry *entry, uint32_t attrib) {
    *entry &= ~attrib; //old: was without ~ !!
}


// Map pde to physical frame (where the appropriate page table stores)
void page_dir_entry_set_frame(page_dir_entry *entry, physical_addres_t addr) {
    *entry = (*entry & ~I86_PDE_FRAME) | addr;
}


bool page_dir_entry_is_present(page_dir_entry entry) {
    return entry & I86_PDE_PRESENT;
}


void flush_tlb_entry(virtual_addr_t addr) {
    asm volatile("invlpg (%0)" : : "b"(addr) : "memory");
}
