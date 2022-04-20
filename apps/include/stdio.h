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

//void printf(char *text, ...);
// или другое

#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_malloc          2
#define SC_CODE_free            3
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33


enum colors  {
    VESA_BLACK = 0x000000,
    VESA_BLUE = 0x0000AA,
    VESA_GREEN = 0x00AA00,
    VESA_CYAN = 0x00AAAA,
    VESA_RED = 0xAA0000,
    VESA_MAGENTA = 0xAA00AA,
    VESA_BROWN = 0xAA5500,
    VESA_LIGHT_GREY = 0xAAAAAA,
    VESA_DARK_GREY = 0x555555,
    VESA_LIGHT_BLUE = 0x5555FF,
    VESA_LIGHT_GREEN = 0x55FF55,
    VESA_LIGHT_CYAN = 0x55FFFF,
    VESA_LIGHT_RED = 0xFF5555,
    VESA_LIGHT_MAGENTA = 0xFF55FF,
    VESA_LIGHT_BROWN = 0xffff55,
    VESA_WHITE = 0xFFFFFF,
};


int getscancode(){
    void* res = 0;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_getscancode));
    asm volatile("int $0x80");

    return res;
}

char getchar(){
    int i = 0;
    int SHIFT = 0;

    while(i <= -1) {
        i = getscancode();
        if (i == 42) {
            SHIFT = 1;
            i = 0;
            continue;
        } else {
            if (SHIFT) {
                return keyboard_map_shifted[(unsigned char) i];
            } else {
                return keyboard_map[(unsigned char) i];
            }
            SHIFT = 0;
        }

    }
} 

char *gets() {
    char temp[256];

    return temp;
} 


int print_str(char *str) {
    void* res = 0;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_puts), "b"(&str));
    asm volatile("int $0x80");
    return res;
}



/*
    putint - вывод числа
*/
void putint(const int i) {
    char res[32];

    if (i < 0) {
        print_str('-');
    }

    itoa(i, res);
    print_str(res);
    
}



void puthex(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned int n, d = 0x10000000;

    print_str("0x");

    while((i / d == 0) && (d >= 0x10)) d /= 0x10;

    n = i;

    while( d >= 0xF ) {
        print_str(hex[n/d]);
        n = n % d;
        d /= 0x10;
    }

    print_str(hex[n]);
}


void print(char *format, va_list args) {
    int i = 0;
    char temp[2] = {0};

    while (format[i]) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's':
                    print_str(va_arg(args, char*));
                    break;
                case 'c':
                    print_str(va_arg(args, int));
                    break;
                case 'd':
                    putint(va_arg(args, int));
                    break;
                case 'i':
                    putint(va_arg(args, int));
                    break;
                case 'u':
                    putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    puthex(va_arg(args, uint32_t));
                    break;
                default:
                    temp[0] = format[i];
                    print_str(temp);
            }
        } else {
            temp[0] = format[i];
            print_str(temp);
        }
        i++;
    }
}


/*
    printf - форматированный вывод
*/
void printf(char *text, ...) {
    va_list args;
    va_start(args, text);
    print(text, args);
}