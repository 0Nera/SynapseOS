#pragma once



#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include <libk/stdio.h>
#include <libk/string.h>
#include <libk/stdlib.h>
#include <libk/list.h>
#include <libk/math.h>

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
#include <drivers/net.h>
#include <drivers/floppy.h>

#include <io/tui.h>
#include <io/tty.h>
#include <io/vgafnt.h>
#include <io/ports.h>
#include <io/imaging.h>

#include <sys/cpuinfo.h>
#include <sys/system.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/tss.h>
#include <sys/syscalls.h>
#include <sys/elf.h>

#define VERSION_MAJOR   0       // Версия ядра
#define VERSION_MINOR   2       // Пре-релиз 
#define VERSION_PATCH   12       // Патч
#define ARCH_TYPE       "i386"   // Архитектура

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

#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

extern uint32_t os_mode;
extern bool multi_task;

void shell();
void sysinfo();
void sbf(char *src);
void cat(char *fname);
void cd(char *dname);
pid_t add_task();
