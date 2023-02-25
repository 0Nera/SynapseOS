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
    kprintf("[%s] at [%x]\n", info->name, info->header->entry);

    if(!(info->header->magic[0] == 0x7f
         && info->header->magic[1] == 'E'
         && info->header->magic[1] == 'L'
         && info->header->magic[1] == 'F')) {
    	debug_log("ELF is invalid!");
    	return -1;
    }
    debug_log("ELF is valid!");
    
    debug_log("\t\tELF file type: %s",
    		  (info->header->file_type == ELF_REL) ? 
        		"relocatable" :
        		(info->header->file_type==ELF_EXEC) ? 
        		  "executable" :
        		  "unknown"
    );
    /*
    char **final_argv = oxygen_alloc(sizeof(char**) * argc);    
    
    final_argv[0] = (char*)info->name;

    for(size_t i = 1; i < argc; i++) {
        final_argv[i] = argv[i-1];
    }
    */

    kprintf("[%s] Loading..\n", info->name);
    int (*entry_point)() = (void*)(info->header->entry);
    kprintf("[%x] entry\n", entry_point);
    //int result = entry_point();

    kprintf("[%s] Return [%d]\n", info->name, -1);
    //oxygen_free(final_argv);
    return -1;
}
