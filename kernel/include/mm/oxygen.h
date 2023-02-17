/**
 * @file oxygen.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Менеджер памяти "Oxygen" для динамичного выделения, доступен также в userspace
 * @version 0.1.0
 * @date 29-01-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2023
 *
 */

#include <arch.h>
#include <kernel.h>
#include <libk.h>
#include <multiboot.h>

#ifndef _OXYGEN_H
#define _OXYGEN_H

struct oxygen_mem_entry {
    struct oxygen_mem_entry* next;
    struct oxygen_mem_entry* prev;
    void* addr;
    size_t size;
};

typedef struct oxygen_mem_entry oxygen_mem_entry_t;

oxygen_mem_entry_t* oxygen_find_free(size_t length);
bool oxygen_multiboot_init(multiboot_info_t* info);
int oxygen_dump_block(oxygen_mem_entry_t* entry);
void oxygen_free(void* ptr);
void* oxygen_alloc(size_t length);
void oxygen_dump_memory();

#endif // oxygen.h