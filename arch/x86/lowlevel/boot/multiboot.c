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


void multiboot_main(multiboot_info_t* info) {
    debug_log("Обработка Multiboot1 заголовка: ");
    debug_log("flags: 0x%x", info->flags);
    debug_log("mem_lower: 0x%x", info->mem_lower);
    debug_log("mem_upper: 0x%x", info->mem_upper);
    debug_log("boot_device: 0x%x", info->boot_device);
    debug_log("Модулей: %u", info->mods_count);
    debug_log("Строка: %s", info->cmdline);
    multiboot_module_t* start = (multiboot_module_t*)info->mods_addr;
    module_syscalls_t syscalls;
    syscalls.printf = kprintf;
    syscalls.debug_log_printf = debug_log_printf;

    if (start) {
        debug_log("end 0x%x, start 0x%x",
            (info->mods_addr + info->mods_count),
            start);
        debug_log("Модули:");
        for (uint32_t i = 0; i < info->mods_count; i++) {
            module_elf_programm_t info;
            info.name = (char*)&start[i].cmdline;
            info.header = (struct elf_header*)start[i].mod_start;

            debug_log("->%s: 0x%x", start[i].cmdline, &start[i]);
            debug_log("\tНачало      =0x%x", start[i].mod_start);
            debug_log("\tКонец       =0x%x", start[i].mod_end);
            debug_log("\tРазмер      =%u", start[i].mod_end - start[i].mod_start);
            debug_log("result %d", elf_module_load(info));
        }
    }
    debug_log("bootloader_name: %s", info->bootloader_name);
}