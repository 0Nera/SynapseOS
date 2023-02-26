/**
 * @file mod.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief 
 * @version 0.1.0
 * @date 22-02-2023
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


#include <arch.h>
#include <libk.h>
#include <kernel.h>


#ifndef _MOD_H
#define _MOD_H 1


/**
 * @brief Типы ELF файлов
 * 
 */
enum elf_type {
    ELF_NONE = 0, 
    ELF_REL  = 1, 
    ELF_EXEC = 2
};


/**
 * @brief 
 * 
 */
typedef struct elf_header {
    uint8_t magic[4];
    uint8_t arch;
    uint8_t byte_order;
    uint8_t elf_version;
    uint8_t os_abi;
    uint8_t reserverd[8];
    uint16_t type;
    uint16_t instruction_set;
    uint32_t machine;
    uint32_t entry;
    uint32_t header_table_position;
    uint32_t section_table_position;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_header_entry_size;
    uint16_t program_header_entries_count;
    uint16_t section_header_entry_size;
    uint16_t section_header_entries_count;
    uint16_t string_table;
}__attribute__((packed)) elf_header_t;

/// @brief 
typedef struct elf_section {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t address;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t address_align;
    uint32_t entry_size;
} __attribute__((packed)) elf_section_t;

typedef struct {
	uint32_t type;
	uint32_t offset;
	uint32_t virtual_addr;
	uint32_t phys_addr;
	uint32_t size;
	uint32_t mem_size;
	uint32_t flags;
	uint32_t align;
} __attribute__((packed)) elf_program_header_t;

typedef struct elf_symbol {
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
} __attribute__((packed)) elf_symbol_t;


typedef struct {
    char *name;
    struct elf_header *header;
	size_t size;
} module_elf_programm_t;


typedef struct {
	void (*printf)(const char *format_string, ...);
	void (*debug_log_printf)(const char *format_string, ...);
} module_syscalls_t;

int elf_module_load(module_elf_programm_t *info/*, size_t argc, char **argv, module_syscalls_t syscalls*/);
void elf_get_info(elf_header_t *header);
elf_program_header_t *elf_get_program_header(elf_header_t *header, int num);

#endif // mod.h