#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <multyboot.h>

#include <mem/mem.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <mem/kheap.h>

#include <drivers/vfs.h>
#include <drivers/ramdisk.h>

#include <io/tty.h>
#include <io/vgafnt.h>
#include <io/ports.h>
#include <drivers/keyboard.h>

#include <interrupts/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/tss.h>
#include <interrupts/syscalls.h>

#include <libk/string.h>
#include <libk/stdlib.h>

#include <sys/elf.h>


#define VERSION_MAJOR   0   // Версия ядра
#define VERSION_MINOR   2   // Пре-релиз 
#define VERSION_PATCH   5   // Патч

#define isDistr         0   // 0 - не дистрибутив, 1 - дистрибутив
#define DistrName       ""  // Имя дистрибутива
#define dVERSION_MAJOR  0   // Версия дистрибутива
#define dVERSION_MINOR  0   // Пре-релиз дистрибутива
#define dVERSION_PATCH  0   // Патч дистрибутива

extern int os_mode;