/**
 * @file oxygen.c
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
#include <mm/oxygen.h>
#include <multiboot.h>

static oxygen_mem_entry_t *first_node;


/**
 * @brief Инициализация менеджера памяти "Кислород"
 *
 * @param address Адрес начала памяти
 * @param length Размер области
 */
void oxygen_init(uintptr_t address, size_t length) {
    debug_log("Инициализация менеджера кучи ядра, %u байт на точку", sizeof(oxygen_mem_entry_t));
    debug_log("Размер области: %u килобайт", length / 1024);
    debug_log("Адрес области: 0x%x", address);
    debug_log("Конец области: 0x%x", address + length - 1);

    first_node = (oxygen_mem_entry_t*)address;

    first_node->size = length - sizeof(oxygen_mem_entry_t);
    first_node->free = true;
    first_node->next = NULL;
}

void oxygen_test() {
    void *temp = oxygen_alloc(1024);
    void *temp1 = oxygen_alloc(1024 * 1024);
    void *temp2 = oxygen_alloc(4096);
    oxygen_dump_memory();

    oxygen_free(temp1);
    oxygen_dump_memory();

    oxygen_free(temp);
    oxygen_dump_memory();

    oxygen_free(temp2);
    oxygen_dump_memory();
}

void oxygen_merge_blocks(oxygen_mem_entry_t* start) {
    if (!start->free) return;
    oxygen_mem_entry_t *block = start;
    while (block->next && block->next->free) {
        block->size += block->next->size + sizeof(oxygen_mem_entry_t);
        block->next = block->next->next;
    }
}

//This function allocates a block of memory.
void *oxygen_alloc(size_t size) {
    return oxygen_alloc_align(size, 1);
}

void* oxygen_alloc_align(size_t size, size_t alignment) {
    oxygen_mem_entry_t *curr = first_node;

    while (curr) {
        if (curr->free) {
            void* addr = curr->data + alignment - 1;
            addr -= (uintptr_t)addr % alignment + sizeof(oxygen_mem_entry_t);
            oxygen_mem_entry_t *second = addr;
            if (curr->size >= (second->data - curr->data + size)) {
                oxygen_mem_entry_t *third = addr + size;

                third->size = curr->size - (third->data - curr->data);
                third->next = curr->next;
                third->free = 1;

                second->size = size;
                second->next = third;
                second->free = 0;

                if (curr != second) {
                    curr->next = second;
                    curr->size = (uintptr_t)second - (uintptr_t)curr->data;
                    curr->free = 1;
                }

                return second->data;
            }
        }

        curr = curr->next;
    }

    return NULL;
}

//This function releases a block of memory.
void oxygen_free(void *ptr) {
    oxygen_mem_entry_t *curr = first_node, *prev = NULL;
    while (curr != NULL) {
        if (curr->data == ptr) {
            curr->free = 1;
            oxygen_merge_blocks(prev ? prev : curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void oxygen_dump_memory() {
    debug_log("Карта блоков:");

    oxygen_mem_entry_t *i = first_node;
    while (i) {
        oxygen_dump_block(i);
        i = i->next;
    }
    debug_log("Конец карты блоков");
}

/**
 * @brief
 *
 * @param entry
 */
void oxygen_dump_block(oxygen_mem_entry_t *entry) {
    debug_log_printf("free %u",
        entry->free);
    debug_log_printf(" | entry 0x%x",
        entry);
    debug_log_printf(" | next 0x%x",
        entry->next);
    debug_log_printf(" | addr 0x%x",
        entry->data);
    debug_log_printf(" | size %u байт ",
        entry->size);
    if (entry->next == NULL) {
        debug_log_printf("LAST ");
    }
    debug_log_printf("(0x%x)\n",
        entry->size);
}
