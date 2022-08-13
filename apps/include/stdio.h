#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <scancodes.h>


/*
    Стандартная библиотека SynapseOS

    stdio - модуль ввода-вывода
*/

#include <vesa.h>


#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_setdev          10
#define SC_CODE_readfile        11
#define SC_CODE_writefile       12
#define SC_CODE_exists          13
#define SC_CODE_filesize        14
#define SC_CODE_rand            20
#define SC_CODE_srand           21
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_version         40
#define SC_CODE_newtask         41
#define SC_CODE_ticks           42
#define SC_CODE_getfreq         43

#include "../include/stdlib.h"
#include "../include/stdint.h"
#include "../include/stdbool.h"

int getscancode();
char getchar();
char *gets();
int getversion();
void print_str(char str[]);
void putint(const int i);
void puthex(unsigned int i);
void putchar(unsigned char ch);
void print(char *format, va_list args);
void printf(char *text, ...);
uint32_t timer_get_ticks();
uint32_t timer_get_freq();

#define EOF -1
#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

#define STDIO_ERR_NO_FOUND 		1	// Файл не найден
#define STDIO_ERR_MODE_ERROR	2	// Режим работы не определён
#define STDIO_ERR_SIZE			3	// Размер файла имеет недопустимый размер
#define STDIO_ERR_NO_OPEN		4	// Файл не был открыт

typedef struct {
	char* path;
    int32_t size;
    uint32_t fmode;
	size_t bufSize;
	char* buf;
	bool open;
	int64_t pos;
	uint32_t err;
} FILE;

// Типы открытого файла, тип флагов rw и т.д.
enum FileOpenMode {
	O_READ = 1,
	O_WRITE = 2,
	O_CREATE = 4,
	O_APPEND = 8,
	O_TRUNC = 16,
};

