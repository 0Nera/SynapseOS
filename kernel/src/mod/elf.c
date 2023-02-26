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

static int (*entry_point)();

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
    	kprintf("ELF is invalid!\n");
    	debug_log("ELF is invalid!");
    	return -1;
    }
    
    elf_get_info(header);
    
	kprintf("Type: %s%s%s\n",
		header->arch        == 1 ? "32bit ":"64 bit",
		header->byte_order  == 1 ? "Little Endian ":"Big endian ",
		header->elf_version == 1 ? "True ELF":"buggy ELF");
	if(header->type != 2) {
		kprintf("File is not executable!\n");
		return 0;
	}

	uint32_t *phys_loc = oxygen_alloc(0x400000);
	elf_program_header_t *ph = (elf_program_header_t *)(header + header->header_table_position);
	
    for(int i = 0; i < header->program_header_entries_count; i++, ph++) {
		switch(ph->type) {
		 	case 0: /* NULL */
		 		break;
		 	case 1: /* LOAD */
		 		kprintf("LOAD: offset 0x%x virtual 0x%x Phys 0x%x size 0x%x mem size 0x%x\n",
		 				ph->offset, ph->virtual_addr, ph->phys_addr, ph->size, ph->mem_size);
		 		debug_log("LOAD: offset 0x%x virtual 0x%x Phys 0x%x size 0x%x mem size 0x%x",
		 				ph->offset, ph->virtual_addr, ph->phys_addr, ph->size, ph->mem_size);
		 		//paging_map(ph->virtual_addr, phys_loc, ph->size);
		 		memcpy(ph->virtual_addr, header + ph->offset, ph->size);
		 		break;
		 	default:
		 	    kprintf("Unsupported elf\n");
		 	    return 0;
		 }
	}
    /*
    char **final_argv = oxygen_alloc(sizeof(char**) * argc);    
    
    final_argv[0] = (char*)info->name;

    for(size_t i = 1; i < argc; i++) {
        final_argv[i] = argv[i-1];
    }
    oxygen_free(final_argv);
    */

    kprintf("[%s] Loading..\n", info->name);
    debug_log("[%s] Loading..", info->name);
    
    entry_point = (void*)(header->entry);
    
    kprintf("[%x] entry\n", entry_point);
    debug_log("[%x] entry", entry_point);

    int result = entry_point();

    kprintf("[%s] Return [%d]\n", info->name, result);
    debug_log("[%s] Return [%d]", info->name, result);
    return result;
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