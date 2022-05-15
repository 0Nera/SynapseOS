#include <kernel.h>

page_directory *kernel_page_dir; // Pointer (physical) to kernel page dircetory structure

uint32_t kv2p(void *a) {
    return (uint32_t)a - 0xC0000000;
}

bool vmm_alloc_page(virtual_addr vaddr) {
    physical_addres paddr = pmm_alloc_block();
    if (!paddr) {
        return false;
    }

    vmm_map_page(paddr, vaddr);
    return true;
}

bool vmm_alloc_page_with_userbit(virtual_addr vaddr) {
    physical_addres paddr = pmm_alloc_block();
    if (!paddr) {
        return false;
    }

    vmm_map_page(paddr, vaddr);
    page_table_entry *pte = GET_PTE(vaddr);
    page_table_entry_add_attrib(pte, I86_PTE_USER);
    return true;
}

void vmm_free_page(virtual_addr vaddr) {
    page_table_entry *pte = GET_PTE(vaddr);
    if (!page_table_entry_is_present(*pte)) {
        tty_printf("oh, you try to delete not present page\n");
        return;
    }

    physical_addres block = page_table_entry_frame(*pte);
    if (block) {
        pmm_free_block(block);
    }
    page_table_entry_del_attrib(pte, I86_PTE_PRESENT);
}

void vmm_create_kernel_page_dir() {
    kernel_page_dir = (page_directory*) pmm_alloc_block();
    if (kernel_page_dir == 0xFFFFFFFF) {
        tty_printf("Failed to allocate phys memory for kernel page dir\n");
        // Panic
        return;
    }

    //page_directory *pd = (page_directory*) vmm_temp_map_page((physical_addres) kernel_page_dir);
    page_directory *pd = kernel_page_dir;
    memset(pd, 0, sizeof(page_directory));

    int32_t i;
    for (i = 0; i < PAGE_ENTRIES; i++) {
        page_dir_entry *pde = (page_dir_entry*) &pd->entries[i];
        page_dir_entry_add_attrib(pde, I86_PTE_WRITABLE);
        page_dir_entry_del_attrib(pde, I86_PTE_PRESENT);

        if (i == PAGE_ENTRIES - 1) { // Fractal(recursive) mapping technique, which allows us to access PD and PT
            page_dir_entry_add_attrib(pde, I86_PTE_PRESENT);
            page_dir_entry_set_frame(pde, (physical_addres) kernel_page_dir);

            //tty_printf("pd[1023] = %x\n", pd->entries[1023]);
        }
    }
}

void vmm_map_page(physical_addres paddr, virtual_addr vaddr) {
    page_dir_entry *pde = GET_PDE(vaddr);
    if (!page_dir_entry_is_present(*pde)) { // If page table isnt present, create it
        physical_addres pt_p = pmm_alloc_block(); // It's phys addr!
        if (pt_p == 0xFFFFFFFF) {
            tty_printf("No free phys memory. Minimum 3 mb\n");
            return;
        }

        page_table *pt_v = (page_table*) vmm_temp_map_page(pt_p); // Because we need to write!
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

virtual_addr vmm_temp_map_page(physical_addres paddr) {
    page_table_entry *pte = GET_PTE(TEMP_PAGE_ADDR);
    page_table_entry_set_frame(pte, PAGE_ALIGN_DOWN(paddr)); // Old:DOWN
    page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
    page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);

    //flush_tlb_entry(TEMP_PAGE_ADDR);
    asm volatile("invlpg %0" :: "m" (*(uint32_t *) TEMP_PAGE_ADDR) : "memory" );
    //flush_tlb_all();

    return TEMP_PAGE_ADDR;
}

// Switch page directory, reveives physical address
void vmm_switch_page_directory(page_directory *page_dir_phys_addr) {
    asm volatile("mov %0, %%cr3" :: "r"((uint32_t) page_dir_phys_addr));
}

void vmm_init() {
    //tty_printf("1\n");

    vmm_create_kernel_page_dir();

    page_table *table1 = (page_table*) pmm_alloc_block();
    page_table *table2 = (page_table*) pmm_alloc_block();

    // Clear allocated page tables
    memset((void*) table1, 0, sizeof(page_table));
    memset((void*) table2, 0, sizeof(page_table));

    // Maps first MB to 3GB
    physical_addres frame;
    virtual_addr virt;
    for (frame = 0x0, virt = 0xC0000000;
         frame < 0x100000/*0x100000*/;
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table1->entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    // Maps kernel pages and phys mem pages
    for (frame = KERNEL_START_PADDR, virt = KERNEL_START_VADDR;
         frame < KERNEL_PHYS_MAP_END;
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table2->entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    page_dir_entry *pde1 = (page_dir_entry*) &kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0x00000000)]; //pdirectory_lookup_entry(cur_directory, 0x00000000);
    page_dir_entry_add_attrib(pde1, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde1, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde1, (physical_addres) table1);

    page_dir_entry **pde2 = (page_dir_entry*) &kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0xC0100000)]; //pdirectory_lookup_entry(cur_directory, 0xC0100000);
    page_dir_entry_add_attrib(pde2, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde2, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde2, (physical_addres) table2);

    update_phys_memory_bitmap_addres(KERNEL_END_VADDR);

    enable_paging((physical_addres) kernel_page_dir);

}

void vmm_test() {
    tty_printf("kernel_page_dir = %x\n", (physical_addres) kernel_page_dir);

    physical_addres padr1 = 0xC0500000;
    virtual_addr vadr1 = vmm_temp_map_page(padr1);
    *(uint8_t*) vadr1 = 77;
    tty_printf("%x = %x\n", padr1, *(uint8_t*) vadr1);

    //IT WILL CAUSE PAGE FAULT!!!! BEACUSE WE 1:1 MAPPED UP TO 1MB PHYS MEM BUT NEVKLYUCHITELNO!
    tty_printf("%x = %x\n", (0x00100000 - 1), *(uint8_t*) (0x00100000 - 1));

    int32_t eip;
    asm volatile("1: lea 1b, %0": "=a"(eip));
    tty_printf("EIP = %x  ", eip);
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
void page_table_entry_set_frame(page_table_entry *entry, physical_addres addr) {
    *entry = (*entry & ~I86_PTE_FRAME) | addr;
}

bool page_table_entry_is_present(page_table_entry entry) {
    return entry & I86_PTE_PRESENT;
}

bool page_table_entry_is_writable(page_table_entry entry) {
    return entry & I86_PTE_WRITABLE;
}

// Return the address of physical frame which pte refers to
physical_addres page_table_entry_frame(page_table_entry entry) {
    return entry & I86_PTE_FRAME;
}

// Functions for Page Directory Entries

// Add attribute to pde
void page_dir_entry_add_attrib(page_dir_entry *entry, uint32_t attrib) {
    *entry |= attrib;
}

// Delete attribute to pde
void page_dir_entry_del_attrib(page_dir_entry *entry, uint32_t attrib) {
    *entry &= ~attrib; //old: was without ~ !!
}

// Map pde to physical frame (where the appropriate page table stores)
void page_dir_entry_set_frame(page_dir_entry *entry, physical_addres addr) {
    *entry = (*entry & ~I86_PDE_FRAME) | addr;
}

bool page_dir_entry_is_present(page_dir_entry entry) {
    return entry & I86_PDE_PRESENT;
}

bool page_dir_entry_is_user(page_dir_entry entry) {
    return entry & I86_PDE_USER;
}

bool page_dir_entry_is_4mb(page_dir_entry entry) {
    return entry & I86_PDE_4MB;
}

bool page_dir_entry_is_writable(page_dir_entry entry) {
    return entry & I86_PDE_WRITABLE;
}

// Return the address of physical frame which pde refers to
physical_addres page_dir_entry_frame(page_dir_entry entry) {
    return entry & I86_PDE_FRAME;
}

void flush_tlb_entry(virtual_addr addr) {
    asm volatile("invlpg (%0)" : : "b"(addr) : "memory");
}
