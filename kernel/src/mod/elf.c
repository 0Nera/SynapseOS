/**
 * @file elf.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Загрузчик модулей которые скомпилированы в формат ELF
 * @version 0.1.0
 * @date 22-02-2023
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2023
 * 
 */


#include <arch.h>
#include <kernel.h>
#include <libk.h>
#include <mod.h>

/**
 * @brief Загрузка и исполнение модуля в формате ELF
 * 
 * @param info Информация о модуле
 * @param argc Количество аргументов
 * @param argv Аргументы
 * @param syscalls Указатель на структуру сисфункций ядра
 * @return int Результат работы модуля
 */
int elf_module_load(module_elf_programm_t *info/*, size_t argc, char **argv,*/) {
    elf_header_t *header = info->header;
    kprintf("[%s] at [0x%x] [%s]\n", info->name, header->entry, header->magic);
    debug_log("[%s] at [0x%x] [%s]", info->name, header->entry, header->magic);

    if(header->magic[0] != 0x7f OR
       header->magic[1] != 'E'  OR
       header->magic[2] != 'L'  OR
       header->magic[3] != 'F') {
    	debug_log("ELF is invalid!");
    	return -1;
    }
    debug_log("ELF is valid!");
    /*
    char **final_argv = oxygen_alloc(sizeof(char**) * argc);    
    
    final_argv[0] = (char*)info->name;

    for(size_t i = 1; i < argc; i++) {
        final_argv[i] = argv[i-1];
    }
    */

    kprintf("[%s] Loading..\n", info->name);
    int (*entry_point)() = (void*)(header->entry);
    kprintf("[%x] entry\n", entry_point);
    //int result = entry_point();

    kprintf("[%s] Return [%d]\n", info->name, -1);
    //oxygen_free(final_argv);
    return -1;
}


void elf_get_info(elf_header_t *header) {
    kprintf("ELF file type: %s\n",
    		  (header->type == ELF_REL) ? 
        		"relocatable" :
        		(header->type==ELF_EXEC) ? 
        		  "executable" :
        		  "unknown"
    );
    kprintf("ELF file version: %u\n", header->elf_version);
    kprintf("Entry point: 0x%x\n", header->entry);
    kprintf("Program header offset: %u\n", header->header_table_position);
    kprintf("Section header offset: %u\n", header->section_table_position);
    kprintf("File flags: %u\n", header->flags);
    kprintf("File header size: %u\n", header->header_size);
    kprintf("Program header entry size: %u\n", header->program_header_entries_count);
    kprintf("Section header entry size: %u\n", header->section_header_entry_size);
    kprintf("Section header count: %u\n", header->section_header_entries_count);
    kprintf("Program header count: %u\n", header->program_header_entries_count);
}