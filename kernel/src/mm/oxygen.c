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

static void *oxygen_mem_start;
static void *oxygen_mem_end;
static size_t oxygen_mem_free;
static size_t oxygen_mem_all;
static oxygen_mem_entry_t *first_node;


/**
 * @brief Инициализация менеджера памяти "Кислород"
 *
 * @param adress Адрес начала памяти
 * @param length Размер области
 * @return bool True в случае успеха
 */
bool oxygen_init(void *adress, size_t length) {
    oxygen_mem_start = adress;
    oxygen_mem_end = adress + length;
    oxygen_mem_all = length;
    oxygen_mem_free = length;

    debug_log("Инициализация менеджера динамичной памяти, %u байт на точку", sizeof(oxygen_mem_entry_t));
    debug_log("Размер области: %u килобайт", length / 1024);
    debug_log("Адрес области: 0x%x", adress);
    debug_log("Конец области: 0x%x", oxygen_mem_end);

    first_node = (oxygen_mem_entry_t*)adress;

    first_node->size = length;
    first_node->free = true;
    first_node->next = NULL;

    void *temp = oxygen_alloc(1024);
    void *temp1 = oxygen_alloc(1024 * 1024);
    void *temp2 = oxygen_alloc(4096);

    oxygen_dump_memory();

    oxygen_free(temp);
    oxygen_free(temp1);
    oxygen_free(temp2);

    return true;
}


//This function allocates a block of memory.
void *oxygen_alloc(size_t size) {
    oxygen_mem_entry_t *curr = first_node;
    
    while (curr != NULL) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            if (curr->size > size) {
                //Split the block into two blocks.
                oxygen_mem_entry_t *new_block = (oxygen_mem_entry_t*)((uintptr_t)curr + size);
                new_block->size = curr->size - size;
                new_block->free = 1;
                new_block->next = curr->next;
                curr->size = size;
                curr->next = new_block;
            }
            return (void *)((uintptr_t)curr + sizeof(oxygen_mem_entry_t));
        }
        curr = curr->next;
    }
    return NULL;
}

//This function releases a block of memory.
void oxygen_free(void *ptr) {
    oxygen_mem_entry_t *curr = first_node;
    while (curr != NULL) {
        if ((void *)((uintptr_t)curr + sizeof(oxygen_mem_entry_t)) == ptr) {
            curr->free = 1;
            break;
        }
        curr = curr->next;
    }
}

void oxygen_dump_memory() {
    debug_log("Карта блоков:");

    oxygen_mem_entry_t *i = first_node;
    while (true) {
        oxygen_dump_block(i);
        if (i->next == NULL) {
            debug_log("Конец карты блоков");
            return;
        }
        i = i->next;
    }
}

/**
 * @brief
 *
 * @param entry
 * @return int
 */
int oxygen_dump_block(oxygen_mem_entry_t *entry) {
    debug_log_printf("free %u",
        entry->free);
    debug_log_printf(" | entry 0x%x",
        entry);
    debug_log_printf(" | next 0x%x",
        entry->next);
    debug_log_printf(" | addr 0x%x",
        entry + sizeof(oxygen_mem_entry_t));
    debug_log_printf(" | size %u байт ",
        entry->size);
    if (entry->next == NULL) {
        debug_log_printf("LAST ");
        debug_log_printf("(0x%x)\n",
            entry->size);
        return -999;
    }
    debug_log_printf("(0x%x)\n",
        entry->size);
    return 0;
}

/**
 * @brief Инициализация
 *
 * @param info
 * @return true
 * @return false
 */
bool oxygen_multiboot_init(multiboot_info_t* info) {
    multiboot_memory_map_t* start = (multiboot_memory_map_t*)info->mmap_addr;
    multiboot_memory_map_t* end = (multiboot_memory_map_t*)(info->mmap_addr + info->mmap_length);
    multiboot_memory_map_t* temp;
    debug_log("Карта памяти:");

    size_t total_free_mem = 0;
    size_t total_used_mem = 0;
    size_t max_len = 0;

    for (multiboot_memory_map_t* entry = start; entry < end; entry++) {
        debug_log("[0x%x]", entry->addr);

        if (max_len < entry->len) {
            max_len = entry->len;
            temp = entry;
        }

        debug_log("\t->%u байт", entry->len);

        if ((entry->len / 1024) > 1) {
            debug_log("\t \\->%u килобайт", entry->len / 1024);
        }

        if ((entry->len / 1024 / 1024) > 1) {
            debug_log("\t  \\->%u мегабайт", entry->len / 1024 / 1024);
        }

        if ((entry->len / 1024 / 1024 / 1024) > 1) {
            debug_log("\t   \\->0x%x гигабайт", entry->len / 1024 / 1024 / 1024);
        }

        switch (entry->type) {
            case 1:
                debug_log("\tСвободно");
                total_free_mem += entry->len;
                break;
            case 2:
                debug_log("\tНе использовать");
                total_used_mem += entry->len;
                break;
            case 3:
                debug_log("\tACPI Reclaim Memory (можно использовать после чтения таблиц ACPI)");
                total_used_mem += entry->len;
                break;
            case 4:
                debug_log("\tACPI NVS");
                total_used_mem += entry->len;
                break;
        }
    }

    debug_log("Самый большой регион памяти: %u килобайт", max_len / 1024);
    debug_log("Всего памяти доступной для использования:");
    debug_log("%u байт", total_free_mem);

    if ((total_free_mem / 1024) > 1) {
        debug_log("\t \\->%u килобайт", total_free_mem / 1024);
    }

    if ((total_free_mem / 1024 / 1024) > 1) {
        debug_log("\t  \\->%u мегабайт", total_free_mem / 1024 / 1024);
    }

    if ((total_free_mem / 1024 / 1024 / 1024) > 1) {
        debug_log("\t   \\->0x%x гигабайт", total_free_mem / 1024 / 1024 / 1024);
    }

    debug_log("Всего памяти недоступной для использования:");
    debug_log("%u байт", total_used_mem);

    if ((total_used_mem / 1024) > 1) {
        debug_log("\t \\->%u килобайт", total_used_mem / 1024);
    }

    if ((total_used_mem / 1024 / 1024) > 1) {
        debug_log("\t  \\->%u мегабайт", total_used_mem / 1024 / 1024);
    }

    if ((total_used_mem / 1024 / 1024 / 1024) > 1) {
        debug_log("\t   \\->0x%x гигабайт", total_used_mem / 1024 / 1024 / 1024);
    }

    return oxygen_init((void*)(uintptr_t)KERNEL_PAGE_TABLE_END, temp->len);
}
