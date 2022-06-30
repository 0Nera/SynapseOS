#pragma once
#include <stdint.h>

// Константы к границам памяти
#define KERNEL_START_PADDR ((uint32_t) &kernel_phys_start)
#define KERNEL_END_PADDR   ((uint32_t) &kernel_phys_end)
#define KERNEL_SIZE        (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNEL_START_VADDR ((uint32_t) &kernel_virt_start)
#define KERNEL_END_VADDR   ((uint32_t) &kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END   kernel_phys_map_end
#define KERNEL_PHYS_MAP_SIZE  kernel_phys_map_end - kernel_phys_map_start;

// Константы для менеджера физической памяти
#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE      4096

// Константы для менеджера виртуальной памяти
#define TEMP_PAGE_ADDR  0xC03FF000 
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR   1024
#define PAGE_SIZE       4096
#define PAGE_ENTRIES    1024

// Константы для менеджера кучи ядра 
#define KHEAP_START_VADDR 0xC0500000 // ИЗМЕНИТЬ ЕСЛИ РАЗМЕР ЯДРА БОЛЕЕ 4MB

// Константы для пользовательской кучи
#define UHEAP_START_VADDR 0x20000000 

#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

extern uint32_t kernel_phys_map_start;
extern uint32_t kernel_phys_map_end;

typedef uint32_t virtual_addr_t;
typedef uint32_t physical_addres_t;