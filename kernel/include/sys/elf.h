#pragma once


#include <stdint.h>
#include <stdbool.h>


#define ELF_ARCH_32BIT (1)
#define ELF_ARCH_64BIT (2)
#define ELF_BYTEORDER_LENDIAN (1)
#define ELF_386_MACHINE (3)
#define SH_UNDEF (0)
#define SEGTYPE_NONE  (0)
#define SEGTYPE_LOAD  (1)
#define SEGTYPE_DLINK (2)
#define SEGTYPE_INTER (3)
#define SEGTYPE_NOTE  (4)

enum elf_type {ELF_NONE = 0, ELF_REL = 1, ELF_EXEC = 2};

struct elf_hdr {
	uint8_t     mag_num[4]; //Should be 0x7f, then "ELF" in ASCII. If it isn't exactly like that, we need to throw an error.
	uint8_t           arch; //ELF file architecture, can be 32- or 64-bit.
	uint8_t     byte_order; //ELF file byte order (little- or big-endian).
	uint8_t        elf_ver; //ELF file version.
	uint8_t         os_abi; //ABI used by this ELF file. OS-specific, we'll use only System V ABI.
	uint8_t        abi_ver; //ABI version
	uint8_t      unused[7];
	uint16_t     file_type; //This contains 0, 1 or 2 - 0 is none, 1 is relocatable file, 2 is executable file.
	uint16_t       machine; //Machine type
	uint32_t       version; //ELF file version.
	uint32_t         entry; //ELF file entry point.
	uint32_t         phoff; //Program header table offset in file.
	uint32_t	     shoff; //Section header table offset in the file.
	uint32_t         flags; //ELF file flags.
	uint16_t         hsize; //File header size.
	uint16_t   ph_ent_size; //Program header entry size.
	uint16_t    ph_ent_cnt; //Program header entry count.
	uint16_t   sh_ent_size; //Section header entry size.
	uint16_t    sh_ent_cnt; //Section header entry count.
	uint16_t sh_name_index; //Index in section header table with section names
} __attribute__((packed));

struct elf_section_header {
	uint32_t	name; //Offset in section name table.
	uint32_t	type; //Section type.
	uint32_t	flags; //Section flags.
	uint32_t	addr;
	uint32_t	offset; //Actual section data offset in the file.
	uint32_t	size;
	uint32_t	link;
	uint32_t	info;
	uint32_t	addralign;
	uint32_t	entsize;
};

struct elf_program_header {
	uint32_t type; //Segment type: 0 - null, 1 - load to load_to address, 2 - requires dynamic linking, 3 - use interpreter, 4 - note.
	uint32_t data_offset; //Segment data offset in the file.
	uint32_t load_to; //Address in (virtual) memory on which segment data should be loaded to.
	uint32_t undefined; //Undefined when using System V ABI.
	uint32_t size_in_file; //Segment size in file.
	uint32_t size_in_mem; //Segment size in memory.
	uint32_t flags; //Flags: 1 - executable, 2 - writable, 4 - readable.
	uint32_t align; //Alignment, must be power of 2.
};

typedef struct elf_program_header elf_program_header_t;
typedef struct elf_section_header elf_section_header_t;


uint8_t elf_check_header(struct elf_hdr *hdr);
elf_section_header_t *elf_get_section_header(void *elf_file, int32_t num);
elf_program_header_t *elf_get_program_header(void *elf_file, int32_t num);
const char *elf_get_section_name(void *elf_file, int32_t num);
void elf_hdr_info(struct elf_hdr *hdr);



void *elf_open(const char*);
void elf_info(const char*);
void elf_info_short(const char*);
int32_t run_elf_file(const char*, int, char**);//, char**, char**, int32_t argc);
