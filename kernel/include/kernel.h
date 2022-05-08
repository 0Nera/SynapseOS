#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <libk/string.h>
#include <libk/stdlib.h>
#include <libk/list.h>

#include <multyboot.h>


#include <mem/mem.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <mem/kheap.h>

#include <drivers/vfs.h>
#include <drivers/ramdisk.h>
#include <drivers/keyboard.h>
#include <drivers/pci.h>
#include <drivers/ata.h>
#include <drivers/time.h>

#include <io/tty.h>
#include <io/vgafnt.h>
#include <io/ports.h>

#include <interrupts/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/tss.h>
#include <interrupts/time.h>
#include <interrupts/syscalls.h>

#include <sys/elf.h>


#define VERSION_MAJOR   0       // Версия ядра
#define VERSION_MINOR   2       // Пре-релиз 
#define VERSION_PATCH   10       // Патч
#define ARCH_TYPE       "x86"   // Архитектура

#define COLOR_TEXT      VESA_LIGHT_GREY
#define COLOR_SYS_TEXT  VESA_LIGHT_CYAN
#define COLOR_SYS_PATH  VESA_LIGHT_GREEN
#define COLOR_ERROR     VESA_LIGHT_RED
#define COLOR_ALERT     VESA_LIGHT_YELLOW

#define isDistr         0   // 0 - не дистрибутив, 1 - дистрибутив
#define DistrName       ""  // Имя дистрибутива
#define dVERSION_MAJOR  0   // Версия дистрибутива
#define dVERSION_MINOR  0   // Пре-релиз дистрибутива
#define dVERSION_PATCH  0   // Патч дистрибутива

struct idt_entry {
    uint16_t handler_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t handler_hi;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

extern int os_mode;

void shell();
void sysinfo();
void sbf(char *src);
void cat(char *fname);
void cd(char *dname);