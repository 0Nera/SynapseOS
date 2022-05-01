#include <mem/mem.h>

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t page_table_entry;
typedef uint32_t page_dir_entry;

typedef struct __attribute__((aligned(4096))) page_directory {
    page_dir_entry entries[PAGES_PER_DIR];
} page_directory;

typedef struct __attribute__((aligned(4096))) page_table {
    page_table_entry entries[PAGES_PER_TABLE];
} page_table;

enum PAGE_PTE_FLAGS {
    I86_PTE_PRESENT       = 1,
    I86_PTE_WRITABLE      = 2,
    I86_PTE_USER          = 4,
    I86_PTE_WRITETHOUGH   = 8,
    I86_PTE_NOT_CACHEABLE = 0x10,
    I86_PTE_ACCESSED      = 0x20,
    I86_PTE_DIRTY         = 0x40,
    I86_PTE_PAT           = 0x80,
    I86_PTE_CPU_GLOBAL    = 0x100,
    I86_PTE_LV4_GLOBAL    = 0x200,
    I86_PTE_FRAME         = 0x7FFFF000
};

enum PAGE_PDE_FLAGS {
    I86_PDE_PRESENT    = 1,
    I86_PDE_WRITABLE   = 2,
    I86_PDE_USER       = 4,
    I86_PDE_PWT        = 8,
    I86_PDE_PCD        = 0x10,
    I86_PDE_ACCESSED   = 0x20,
    I86_PDE_DIRTY      = 0x40,
    I86_PDE_4MB        = 0x80,
    I86_PDE_CPU_GLOBAL = 0x100,
    I86_PDE_LV4_GLOBAL = 0x200,
    I86_PDE_FRAME      = 0x7FFFF000
};

#define PAGE_ALIGN_DOWN(x)     ((x) & -PAGE_SIZE)
#define PAGE_ALIGN_UP(address) (((address) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1) )

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x)     (((x) >> 12) & 0x3FF)

#define PAGE_GET_TABLE_ADDRESS(x)    (*x & ~0xFFF) 
#define PAGE_GET_physical_addresESS(x) (*x & ~0xFFF) 

#define GET_PDE(v) (page_dir_entry*) (0xFFFFF000 +  (v >> 22) * 4) 
#define GET_PTE(v) (page_table_entry*) (0xFFC00000 + (v >> 12) * 4)

extern page_directory *kernel_page_dir;


void page_table_entry_add_attrib(page_table_entry *entry, uint32_t attrib);
void page_table_entry_del_attrib(page_table_entry *entry, uint32_t attrib); 
void page_table_entry_set_frame(page_table_entry *entry, physical_addres addr); 
bool page_table_entry_is_present(page_table_entry entry);
bool page_table_entry_is_writable(page_table_entry entry);
physical_addres page_table_entry_frame(page_table_entry entry);

void page_dir_entry_add_attrib(page_dir_entry *entry, uint32_t attrib); 
void page_dir_entry_del_attrib(page_dir_entry *entry, uint32_t attrib); 
void page_dir_entry_set_frame(page_dir_entry *entry, physical_addres addr);
bool page_dir_entry_is_present(page_dir_entry entry);
bool page_dir_entry_is_user(page_dir_entry entry);
bool page_dir_entry_is_4mb(page_dir_entry entry);
bool page_dir_entry_is_writable(page_dir_entry entry);
physical_addres page_dir_entry_frame(page_dir_entry entry); 

void flush_tlb_entry(virtual_addr addr);

extern void enable_paging(physical_addres page_dir);

void vmm_init();
void vmm_create_kernel_page_dir();

bool vmm_alloc_page(virtual_addr vaddr);
bool vmm_alloc_page_with_userbit(virtual_addr vaddr);
void vmm_free_page(virtual_addr vaddr);
void vmm_map_page(physical_addres paddr, virtual_addr vaddr);
virtual_addr vmm_temp_map_page(physical_addres paddr);
void vmm_switch_page_directory(page_directory *page_dir_phys_addr);

void vmm_test();