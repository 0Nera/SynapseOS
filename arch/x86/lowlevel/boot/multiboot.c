/**
 * @file multiboot.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief 
 * @version 0.1.0
 * @date 22-02-2023
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2023
 * 
 */

#include <kernel.h>
#include <multiboot.h> // Multiboot1
#include <mod.h>

extern kernel_info_t kernel_info;

static void multiboot_dump_mem(multiboot_info_t *info) {
    multiboot_memory_map_t *start = (multiboot_memory_map_t*)info->mmap_addr;
    multiboot_memory_map_t *end = (multiboot_memory_map_t*)(info->mmap_addr + info->mmap_length);
    debug_log("Карта памяти:");

    size_t total_free_mem = 0;
    size_t total_used_mem = 0;
    size_t max_len = 0;

    for (multiboot_memory_map_t *entry = start; entry < end; entry++) {
        debug_log("[0x%x]", entry->addr);

        if (max_len < entry->len) {
            max_len = entry->len;
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
}

void multiboot_main(multiboot_info_t *info) {
    debug_log("Обработка Multiboot1 заголовка: ");
    debug_log("flags: 0x%x", info->flags);
    debug_log("mem_lower: 0x%x", info->mem_lower);
    debug_log("mem_upper: 0x%x", info->mem_upper);
    debug_log("boot_device: 0x%x", info->boot_device);
    debug_log("Модулей: %u", info->mods_count);
    debug_log("Строка: %s", info->cmdline);
    multiboot_module_t *start = (multiboot_module_t*)info->mods_addr;
    module_syscalls_t syscalls;
    syscalls.printf = kprintf;
    syscalls.debug_log_printf = debug_log_printf;
    multiboot_dump_mem(info);

    if (start) {
        debug_log("end 0x%x, start 0x%x",
            (info->mods_addr + info->mods_count),
            start);
        debug_log("Модули:");
        for (uint32_t i = 0; i < info->mods_count; i++) {
            module_elf_programm_t info;
            info.name = (char*)start[i].cmdline;
            info.header = (struct elf_header*)start[i].mod_start;
            info.size = start[i].mod_end - start[i].mod_start;
            debug_log("->%s: 0x%x", start[i].cmdline, &start[i]);
            debug_log("\tНачало      =0x%x", start[i].mod_start);
            debug_log("\tКонец       =0x%x", start[i].mod_end);
            debug_log("\tРазмер      =%u", start[i].mod_end - start[i].mod_start);
            debug_log("result %d", elf_module_load(&info, &syscalls));
        }
    }
    debug_log("bootloader_name: %s", info->bootloader_name);
}
