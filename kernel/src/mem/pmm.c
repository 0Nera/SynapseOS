/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/


#include <kernel.h>


uint32_t *phys_memory_bitmap = 0;
uint32_t phys_block_count = 0;
uint32_t phys_used_block_count = 0;

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

uint64_t phys_installed_memory_size = 0, phys_available_memory_size = 0;

// Для рамдиска
uint64_t initrd_mmap_entry_addr = -1;
uint32_t mmap_avail_entries_count = 0;
multiboot_memory_map_entry mmap_avail_entries_array[100];


inline static void bitmap_set(int32_t bit) {
    phys_memory_bitmap[bit / 32] |= (1 << (bit % 32));
}


inline static void bitmap_unset(int32_t bit) {
    phys_memory_bitmap[bit / 32] &= ~(1 << (bit % 32));
}


inline static bool bitmap_test(int32_t bit) {
    return phys_memory_bitmap[bit / 32] & (1 << (bit % 32));
}

uint64_t getInstalledRam(){
    return phys_installed_memory_size;
}

uint64_t getAvailableRam(){
    return phys_available_memory_size;
}

uint64_t getUsedRam(){
    return phys_installed_memory_size-phys_available_memory_size;
}


void pmm_parse_memory_map(multiboot_memory_map_entry *mmap_addr, uint32_t length) {
    multiboot_memory_map_entry *mentry = 0;	

    int32_t n = length / sizeof(multiboot_memory_map_entry); // Количество записей в структуре карты памяти

    mentry = mmap_addr;                                      // Указатель на карту памяти

    for (int32_t i = 0; i < n; i++) {
        if ((mentry + i)->type == 1) {
            phys_available_memory_size += (mentry + i)->len;
        }
        phys_installed_memory_size += (mentry + i)->len;
    }

    
    tty_printf("Installed memory size: %d KB", phys_installed_memory_size / 1024);
    tty_printf(" = %d MB\n", phys_installed_memory_size / (1024 * 1024));

    tty_printf("Available memory size: %d KB", phys_available_memory_size / 1024);
    tty_printf(" = %d MB\n", phys_available_memory_size / (1024 * 1024));
    
    if (phys_available_memory_size / 1024 < 4600) {
        tty_setcolor(COLOR_ERROR);
        tty_printf("\nWarning! Low memory! Using 0 mode.\n");
        os_mode = 1;
    } else if (phys_available_memory_size / 1024 < 13000) {
        tty_setcolor(VESA_LIGHT_GREEN);
        tty_printf("\nStable. Using normal mode.\n");
        os_mode = 1;
    } else if (phys_available_memory_size / 1024 >= 13000) {
        tty_setcolor(VESA_LIGHT_MAGENTA);
        tty_printf("\nHigh mem. Using server mode.\n");
        os_mode = 2;
    } else {
        tty_setcolor(VESA_LIGHT_GREEN);
        tty_printf("\nUnknown. Using normal mode.\n");
        os_mode = 1;
    }
    tty_setcolor(COLOR_SYS_TEXT);
}


int32_t pmm_find_free_block() {
    for (uint32_t i = 0; i < phys_block_count / 32; i++) {
        uint32_t block = phys_memory_bitmap[i];
        if (block != 0xFFFFFFFF) {
            for (uint8_t j = 0; j < 32; j++) {
                int32_t bit = 1 << j;
                if (!(bit & block)) {
                    return (32 * i) + j;
                }
            }
        }
    }
    return -1;
}

int32_t pmm_find_free_blocks(uint32_t count) {
    int32_t starting_block = -1;
    int32_t starting_block_bit = -1;
    uint32_t cur_block_num = 0;

    for (uint32_t i = 0; i < phys_block_count / 32; i++) {
        uint32_t cur_block = phys_memory_bitmap[i];
        if (cur_block == 0xFFFFFFFF) {
            cur_block_num = 0;
            continue;
        }

        for (uint8_t j = 0; j < 32; j++) {
            int32_t bit = 1 << j;
            if (bit & cur_block) { // Бит установлен
                cur_block_num = 0;
                continue;
            }

            if (!cur_block_num) {
                starting_block = i;
            }
            if (!cur_block_num) {
                starting_block_bit = j;
            }

            cur_block_num += 1;
            if (cur_block_num == count) {
                return (32 * starting_block) + starting_block_bit;
            }
        }
    }
    return -1;
}

// Функции для управления одним блоком в памяти
physical_addres_t pmm_alloc_block() {
    if (phys_block_count - phys_used_block_count <= 0) {
        return 0xFFFFFFFF;
    }

    int32_t free_block = pmm_find_free_block();
    if (free_block == -1) {
        return 0xFFFFFFFF;
    }

    bitmap_set(free_block);
    uint32_t addr = free_block * PHYS_BLOCK_SIZE;
    phys_used_block_count++;

    return addr;
}


void pmm_free_block(physical_addres_t addr) {
    int32_t block = addr / PHYS_BLOCK_SIZE;

    bitmap_unset(block);
    phys_used_block_count--;
}


bool pmm_is_block_alloced(physical_addres_t addr) {
    int32_t block = addr / PHYS_BLOCK_SIZE;

    return bitmap_test(block);
}


// Функции для выделения нескольких блоков памяти
physical_addres_t pmm_alloc_blocks(uint32_t count) {
    if (phys_block_count - phys_used_block_count <= 0) {
        return 0xFFFFFFFF; 
    }

    int32_t free_block = pmm_find_free_blocks(count);

    if (free_block == -1) {
        return 0xFFFFFFFF; 
    }

    for (uint32_t i = 0; i < count; i++) {
        bitmap_set(free_block + i);
    }

    uint32_t addr = free_block * PHYS_BLOCK_SIZE;
    phys_used_block_count += count;
    return addr;
}


void pmm_free_blocks(physical_addres_t addr, uint32_t count) {
    int32_t block = addr / PHYS_BLOCK_SIZE;

    for (uint32_t i = 0; i < count; i++) {
        bitmap_unset(block + i);
    }
    phys_used_block_count -= count;
}


// Внутренние функции для выделения диапазонов памяти
void pmm_alloc_chunk(uint64_t base_addr, uint64_t length) {
    int32_t cur_block_addr = base_addr / PHYS_BLOCK_SIZE;
    int32_t num_blocks = length / PHYS_BLOCK_SIZE;

    while (num_blocks-- >= 0) {
        bitmap_set(cur_block_addr++);
        phys_used_block_count--;
    }
}


void pmm_free_chunk(uint64_t base_addr, uint64_t length) {
    int32_t cur_block_addr = base_addr / PHYS_BLOCK_SIZE;
    int32_t num_blocks = length / PHYS_BLOCK_SIZE;

    while (num_blocks--) {
        bitmap_unset(cur_block_addr++);
        phys_used_block_count--;
    }
}


// Функции для инициализации диспетчера физической памяти
void pmm_free_available_memory(struct multiboot_info *mb) {
    multiboot_memory_map_entry *mm = (multiboot_memory_map_entry*) mb->mmap_addr;

    while ((unsigned int) mm < mb->mmap_addr + mb->mmap_length) {
        if (mm->type == 1) { 
            if (mm->addr != initrd_mmap_entry_addr) { 
                pmm_free_chunk(mm->addr, mm->len);
            } else {
                uint32_t initrd_beg = *(uint32_t*) (mb->mods_addr);
                uint32_t initrd_end = *(uint32_t*) (mb->mods_addr + 4);
                uint32_t initrd_size = initrd_end - initrd_end;
                pmm_alloc_chunk(initrd_beg, initrd_size);
                pmm_free_chunk(mm->addr, mm->len - initrd_size - 2);
            }
        }
        mm = (multiboot_memory_map_entry*) ((unsigned int) mm + mm->size + sizeof(mm->size));
    }
    bitmap_set(0);
}

void pmm_relocate_initrd_to_high_mem(struct multiboot_info *mb) {
    uint32_t initrd_beg = *(uint32_t*) (mb->mods_addr);
    uint32_t initrd_end = *(uint32_t*) (mb->mods_addr + 4);
    uint32_t initrd_size = initrd_end - initrd_beg;

    multiboot_memory_map_entry *mm = (multiboot_memory_map_entry*) mb->mmap_addr;
    mmap_avail_entries_count = 0;

    while ((unsigned int) mm < mb->mmap_addr + mb->mmap_length) {
        if (mm->type == 1) { 
            mmap_avail_entries_array[mmap_avail_entries_count] = *mm;
            mmap_avail_entries_count++;
        }
        mm = (multiboot_memory_map_entry*) ((unsigned int) mm + mm->size + sizeof(mm->size));
    }

    for (int32_t i = mmap_avail_entries_count - 1; i >= 0; i--) {
        if (mmap_avail_entries_array[i].len >= initrd_size) {
            initrd_mmap_entry_addr = mmap_avail_entries_array[i].addr;
            memcpy(
                (void*)(intptr_t)(
                    initrd_mmap_entry_addr + mmap_avail_entries_array[i].len - initrd_size - 1
                    ), 
                (void*)initrd_beg, 
                (size_t)initrd_size
                );
            initrd_beg = initrd_mmap_entry_addr + mmap_avail_entries_array[i].len - initrd_size - 1;
            initrd_end = initrd_beg + initrd_size;
            break;
        }
    }

    *(uint32_t*) (mb->mods_addr) = initrd_beg;
    *(uint32_t*) (mb->mods_addr + 4) = initrd_end;
}


void pmm_init(struct multiboot_info *mboot_info) {
    multiboot_memory_map_entry *mmap = (multiboot_memory_map_entry*) mboot_info->mmap_addr;
    pmm_parse_memory_map(mmap, mboot_info->mmap_length);                        // Он также вычисляет phys_installed_memory_size

    pmm_relocate_initrd_to_high_mem(mboot_info);

    phys_block_count = (phys_installed_memory_size) / PHYS_BLOCK_SIZE;          // Сколько блоков будет
    phys_used_block_count = phys_block_count;                                   // Изначально используются все блоки
    phys_memory_bitmap = (uint32_t*) KERNEL_END_PADDR;                          // Битовая карта физической памяти начинается после ядра
    memset(phys_memory_bitmap, 0xFF, phys_block_count / PHYS_BLOCKS_PER_BYTE);  // Изначально помечаем всю установленную память как используемую
    pmm_free_available_memory(mboot_info);                                      // Освобождает память, которую GRUB считает доступной
    pmm_alloc_chunk(KERNEL_START_PADDR, KERNEL_SIZE);                           // Из освобожденной памяти нам нужно выделить те, которые используются ядром
    
    pmm_alloc_chunk((uint64_t)(intptr_t)phys_memory_bitmap, phys_block_count);  // Нам также необходимо выделить память, используемую самой физической картой.
    kernel_phys_map_start = (uint32_t) phys_memory_bitmap;
    kernel_phys_map_end = kernel_phys_map_start + (phys_block_count / PHYS_BLOCKS_PER_BYTE);
    qemu_log("PMM installed");
}


void update_phys_memory_bitmap_addres(physical_addres_t addr) {
    phys_memory_bitmap = (uint32_t*) addr;
}
