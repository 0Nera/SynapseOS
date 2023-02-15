/**
 * @file draw.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции рисования
 * @version 0.1.0
 * @date 02-12-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */

#include <kernel.h>
#include <libk.h>
#include <arch.h>


extern canvas_t *kernel_canvas;
uint32_t psf_font_version = 0;

static psf_t *_font_ptr = NULL;
static bool _init = false;
static uint8_t _width = 8;
static uint8_t _height = 0;
static uint16_t screen_x = 0;
static uint16_t screen_y = 0;
static uint32_t screen_color = 0x55FFFF;

bool text_init() {
    psf_t *header = (psf_t*)(&_binary_kernel_src_graf_font_psf_start);
    screen_x = 0;
    screen_y = 0;
    _init = false;
    _width = 0;
    _height = 0;

    if (header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1) {
        debug_log("ERROR");
        return false;
    }
    _font_ptr =  (psf_t*)(&_binary_kernel_src_graf_font_psf_start);
    _width = 8;
    _height = header->charheight;
    _init = true;
    debug_log("_width %d, _height %d, mode %d", _width, _height, header->mode);
    return _init;
}

size_t psf1_get_width() {
    return _width;
}


size_t psf1_get_height() {
    return _height;
}


unsigned char *psf1_get_glyph(int16_t character) {
    psf_t *header = (psf_t*)_font_ptr;

    if ((character > 511) || (character > 255 && (header->mode == 0 || header->mode == 2))) {
        //debug_log("DEAD");
        //return 0xDEAD;
    }

    return ((unsigned char*)_font_ptr) + sizeof(psf_t) + (character * _height);
}


void draw_vga_character(int16_t c, int pos_x, int pos_y, int color) {
    int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    unsigned char  *glyph = psf1_get_glyph(c);
    //debug_log("[%c] %u %u", c, pos_x, pos_y);

    for (size_t y = 0; y < psf1_get_height(); y++){
        for (size_t x = 0; x < 0 + psf1_get_width(); x++){
            if (glyph[y] & mask[x]){
                vesa_put_pixel(pos_x+x,pos_y+y,color);
            }
        }
    }
}

void draw_text_string(const char *text, int len, int x, int y, int color) {

    for (int i = 0; i < len; i++) {
        if (x + 8 <= 1024) {
            draw_vga_character(text[i], x, y, color);
            x += 8;
        } else {
            break;
        }
    }
}


void text_putc(char c) {
    switch(c) {
        case '\n':
            screen_x = 0;
            screen_y += _height;
            return;
        case '\t':
            screen_x += 4 * 8;
            return;
        default:
            break;
    }
    if(screen_x > kernel_canvas->width) {
        screen_x = 0;
        screen_y += _height;
    }
    draw_vga_character(c, screen_x, screen_y, screen_color);
    screen_x += _width;
}


void text_puts(char *str) {
    while(*str) {
        text_putc(*str);
        str++;
    }
}


/**
 * @brief Вывод шестнадцатеричного числа в COM1 порт
 * 
 * @param num Выводимое число
 */
void text_printudec(int32_t i) {
    uint32_t n, d = 1000000000;
    char str[255];
    uint32_t dec_index = 0;

    while ((i / d == 0) && (d >= 10)) {
        d /= 10;
    }
    n = i;

    while (d >= 10) {
        str[dec_index++] = ((char) ((int) '0' + n / d));
        n = n % d;
        d /= 10;
    }

    str[dec_index++] = ((char) ((int) '0' + n));
    str[dec_index] = 0;
    text_puts(str);
}

void text_printdec(int32_t i) {
    if (i >= 0) {
        text_printudec(i);
    } else {
        text_putc('-');
        text_printudec(-i);
    }
}


/**
 * @brief 
 * 
 * @param i 
 */
void text_printhex(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;

    text_puts("0x");

    while ((i / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = i;

    while (d >= 0xF) {
        text_putc(hex[n / d]);
        n = n % d;
        d /= 0x10;
    }
    text_putc(hex[n]);
}


/**
 * @brief 
 * 
 * @param i 
 */
void text_printhex_v(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;


    while ((i / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = i;

    while (d >= 0xF) {
        text_putc(hex[n / d]);
        n = n % d;
        d /= 0x10;
    }
    text_putc(hex[n]);
}


/**
 * @brief 
 * 
 * @param address 
 * @param size 
 */
void text_dump(void *address, uint32_t size) {
    /* Преобразуем указатель в массив байт */
    uint8_t *dump = (uint8_t*) address; 
    /* Преобразуем указатель просто в число */  
    uint32_t addr_tmp = (uint32_t) address;
    /* Вспомогательные переменные */
    uint32_t mark = 0;
    
    for (uint32_t i = 0; i < size; i++) {
    
        if (mark == 0) {
            text_printhex(addr_tmp);
            text_puts(": ");
        }
    
        text_printhex(dump[i]);
    
        switch(mark) {
            case 7:
                text_putc('|');
                mark++;
                break;
            case 16:
                text_putc('\n');
                mark = 0;
                addr_tmp += 0x10;
                break;
            default:
                text_putc(' ');
                mark++;
                break;
        }
    }
    text_putc('\n');
}



static int text_powk(int x, unsigned int y) {
    if( y == 0)
        return 1;
    else if (y%2 == 0)
        return text_powk(x, y/2)*text_powk(x, y/2);
    else
        return x*text_powk(x, y/2)*text_powk(x, y/2);
 
}
 

static void text_printfloat(double num, int after_point) {
    int int_float = (int) num;
    text_printdec(int_float);
    text_putc('.');
    
    int mut_ab = text_powk(10,after_point);
    int after_point_float = (int) (num * mut_ab); //33314
    
    int multiplier = text_powk(10,(after_point-1));
    //printf("MULTIPLIER : %d\n" , multiplier);
    for(int xy=0; xy<after_point; xy++) {
        int muted = (after_point_float/multiplier);
        //printf("%d\n",muted);
        muted = muted % 10;
        text_printdec(muted);
        
        multiplier/=10;
    }
}



/**
 * @brief Вывод на экран форматированной строки используя неопределенное количество аргументов
 * 
 * @param format_string Строка форматов
 * @param length 
 * @param x 
 * @param y 
 * @param ... Аргументы
 */
void text_printf(const char *format_string, size_t length, uint32_t x, uint32_t y, ...) {
    va_list args;

    // Ищем первый аргумент
    va_start(args, format_string);

    // Обработка и парсинг строки форматов
    while (*format_string != '\0') {
        if (*format_string == '%') {
            format_string++;
            if (*format_string == 'x') {
                text_printhex(va_arg(args, unsigned long long));
            } else if (*format_string == '8') {
                text_printdec((unsigned char)va_arg(args, char*)[0]);
            } else if (*format_string == 'd') {
                text_printdec(va_arg(args, int));
            } else if (*format_string == 'l') {
                text_printdec(va_arg(args, long));
            } else if (*format_string == 'f') {
                text_printfloat(va_arg(args, double), 7);
            } else if (*format_string == 'u') {
                text_printudec(va_arg(args, unsigned int));
            } else if (*format_string == 's') {
                text_puts(va_arg(args, char*));
            } else if (*format_string == 'c') {
                text_putc((char)va_arg(args, int));
            }
        } else {
            text_putc(*format_string);
        }
        format_string++;
    }
    
    // Освобождаем память
    va_end(args);
}