#include <multiboot.h>  // Multiboot1
#include <kernel.h>


extern kernel_info_t kernel_info;


void multiboot_main(multiboot_info_t *info) {
    debug_log("Обработка Multiboot1 заголовка: ");
    debug_log("flags: %x", info->flags);
    debug_log("mem_lower: %x", info->mem_lower);
    debug_log("mem_upper: %x", info->mem_upper);
    debug_log("boot_device: %x", info->boot_device);
    debug_log("Модулей: %u", info->mods_count);
    debug_log("Строка: %s", info->cmdline);
    {
        multiboot_module_t *start = (multiboot_module_t*)info->mods_addr;
        multiboot_module_t *entry = (multiboot_module_t*)start;
        debug_log("entry %x, end %x, start %x", 
            entry, 
            (info->mods_addr + info->mods_count), 
            start);
        debug_log("Модули:");
        entry = start;
        size_t i = 0;
        do {
            if (entry == NULL) {
                goto pars_end;
            }

            debug_log("->%s: %x", entry->cmdline, entry);
            debug_log("\tНачало      =%x", entry->mod_start);
            debug_log("\tКонец       =%x", entry->mod_end);
            debug_log("\tРазмер      =%u", entry->mod_end - entry->mod_start);
            entry++;
            i++;
        } while (i != info->mods_count);
        
    }
    pars_end:
    debug_log("bootloader_name: %s", info->bootloader_name);
}