/**
 * @file paging.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Страничная адресация
 * @version 0.1.0
 * @date 06-02-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <arch.h>
#include <libk.h>
#include <mm/oxygen.h>

uintptr_t __attribute__((aligned(4096))) kernel_page_dir[1024] = {0};
static uint32_t __attribute__((aligned(4096))) kernel_page_table[1024] = {0};
static uint32_t __attribute__((aligned(4096))) kernel_heap_page_table[1024] = {0};
static uint32_t __attribute__((aligned(4096))) kernel_temp_page_table[1024] = {0};
static uint32_t __attribute__((aligned(4096))) kernel_app_page_table[1024] = {0};
static uint32_t __attribute__((aligned(4096))) kernel_app2_page_table[1024] = {0};

// FIXME: сломается если область памяти переходит границу 4мб
void paging_identity_map(uintptr_t addr, uint32_t size) {
    addr &= ~0x3ff;
    uint32_t* page_table;
    if (!kernel_page_dir[addr >> 22]) {
        page_table = oxygen_alloc_align(4096, 4096);
        kernel_page_dir[addr >> 22] = (uintptr_t)page_table | 3;
    } else {
        page_table = (void*)(kernel_page_dir[addr >> 22] & ~3);
    }
    for (uint32_t i = addr; i < (addr + size); i += 4096) {
        page_table[(i >> 12) & 0x3ff] = i | 3;
    }
    asm("mov %cr3, %eax\n"
        "mov %eax, %cr3");
}

void paging_init() {
    for (uint32_t i = 0; i < 1024; i++) {
        kernel_page_table[i] = (i * 4096) | 3;
        kernel_heap_page_table[i] = ((i + 1024) * 4096) | 3;
        kernel_temp_page_table[i] = ((i + 1024) * 4096) | 3;
        kernel_app_page_table[i]  = ((i + 1024) * 4096) | 3;
        kernel_app2_page_table[i]  = ((i + 1024) * 4096) | 3;
    }

    kernel_page_dir[0] = (uintptr_t)(kernel_page_table) | 3;
    kernel_page_dir[1] = (uintptr_t)(kernel_heap_page_table) | 3;
    kernel_page_dir[2] = (uintptr_t)(kernel_temp_page_table) | 3;
    kernel_page_dir[3] = (uintptr_t)(kernel_app_page_table) | 3;
    kernel_page_dir[4] = (uintptr_t)(kernel_app2_page_table) | 3;

    asm("movl %%ecx, %%cr3\n"
        "movl %%cr0, %%eax\n"
        "orl $0x80000000, %%eax\n"
        "movl %%eax, %%cr0"
        :: "c"(kernel_page_dir));
}
