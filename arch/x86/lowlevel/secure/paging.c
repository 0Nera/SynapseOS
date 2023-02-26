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

uint32_t __attribute__((aligned(4096))) kernel_page_dir[1024] = {0};
uint32_t __attribute__((aligned(4096))) kernel_page_table[1024] = {0};
uint32_t __attribute__((aligned(4096))) kernel_heap_page_table[1024] = {0};

// FIXME: сломается если область памяти переходит границу 4мб
void paging_map(uintptr_t addr, uintptr_t vaddr, uint32_t size) {
    vaddr &= ~0x3ff;
    addr &= ~0x3ff;
    uint32_t* page_table;
    if (!kernel_page_dir[vaddr >> 22]) {
        page_table = oxygen_alloc_align(4096, 4096);
        kernel_page_dir[vaddr >> 22] = ((uintptr_t)page_table - 0xC0000000) | 3;
    }
    else {
        page_table = (void*)((kernel_page_dir[vaddr >> 22] & ~3) + 0xC0000000);
    }
    for (uint32_t i = addr; i < (addr + size); i += 0x1000) {
        page_table[(vaddr >> 12) & 0x3ff] = i | 3;
        vaddr += 0x1000;
    }
    asm("mov %cr3, %eax\n"
        "mov %eax, %cr3");
}

void paging_identity_map(uintptr_t addr, uint32_t size) {
    paging_map(addr, addr, size);
}

void paging_unmap_pt(uintptr_t addr) {
    kernel_page_dir[addr >> 22] = 0;
    asm("mov %cr3, %eax\n"
        "mov %eax, %cr3");
}

void paging_init() {
    kernel_page_dir[0xc0400000 >> 22] = ((uintptr_t)kernel_heap_page_table - 0xc0000000) | 3;
    paging_map(0x400000, 0xc0400000, 0x400000);
}
