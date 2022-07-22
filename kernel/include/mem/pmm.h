#include <mem/mem.h>

void pmm_init(struct multiboot_info *mboot_info);
physical_addres_t pmm_alloc_block();
void pmm_free_block(physical_addres_t addr);
void update_phys_memory_bitmap_addres(physical_addres_t addr);