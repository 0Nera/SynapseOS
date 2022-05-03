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





int getscancode();
char getchar();
char *gets();
int getversion();
int print_str(char *str);
void putint(const int i);
void puthex(uint32_t i);
void print(char *format, va_list args);
void printf(char *text, ...);