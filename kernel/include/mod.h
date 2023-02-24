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
struct elf_header {
	uint8_t      magic[4]; ///< Сначала идет 0x7f, потов "ELF" в ASCII.
	uint8_t           arch; ///< Архитектура 32/64
	uint8_t     byte_order; ///< 
	uint8_t    elf_version; ///< 
	uint8_t         os_abi; ///< 
	uint8_t        abi_ver; ///< 
	uint8_t      unused[7]; ///< 
	uint16_t     file_type; ///< 
	uint16_t       machine; ///< 
	uint32_t       version; ///< 
	uint32_t         entry; ///< 
} __attribute__((packed));


typedef struct {
    char *name;
    struct elf_header *header;
} module_elf_programm_t;


typedef struct {
	void (*printf)(const char *format_string, ...);
	void (*debug_log_printf)(const char *format_string, ...);
} module_syscalls_t;

int elf_module_load(module_elf_programm_t *info/*, size_t argc, char **argv, module_syscalls_t syscalls*/);

#endif // mod.h