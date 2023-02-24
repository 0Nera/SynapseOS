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

#include <arch.h>
#include <drivers/vesa.h>
#include <kernel.h>
#include <libk.h>
#include <print.h>

extern canvas_t * kernel_canvas;

static psf_t * _font_ptr = NULL;
static bool _init = false;
static uint8_t _width = 8;
static uint8_t _height = 0;
static uint16_t screen_x = 0;
static uint16_t screen_y = 0;
static uint32_t screen_color = 0x55FFFF;


bool text_init() {
    _font_ptr = &_binary_kernel_src_graf_font_psf_start;

    if (_font_ptr->magic[0] != PSF1_MAGIC0 || _font_ptr->magic[1] != PSF1_MAGIC1) {
        debug_log("ERROR");
        return false;
    }
    _height = _font_ptr->charheight;
    _init = true;
    debug_log("_width %u, _height %u, mode %u", _width, _height, _font_ptr->mode);
    return _init;
}


unsigned char *psf1_get_glyph(int16_t character) {
    if ((_font_ptr->mode == 1 && character < 512) || character < 256)
        return ((unsigned char*)_font_ptr) + sizeof(psf_t) + (character * _height);

    return NULL;
}


void draw_vga_character(int16_t c, int pos_x, int pos_y, int color) {
    unsigned char *glyph = psf1_get_glyph(c);
    if (!glyph)
        return;

    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            if (glyph[y] & (1 << (_width - x))) {
                vesa_put_pixel(pos_x + x, pos_y + y, color);
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
    switch (c) {
        case '\n':
            screen_x = 0;
            screen_y += _height;
            return;
        case '\t':
            screen_x += 4 * _width;
            return;
        default:
            break;
    }
    if (screen_x > kernel_canvas->width) {
        screen_x = 0;
        screen_y += _height;
    }
    draw_vga_character(c, screen_x, screen_y, screen_color);
    screen_x += _width;
}


/**
 * @brief Вывод на экран форматированной строки используя неопределенное количество аргументов
 *
 * @param format_string Строка форматов
 * @param length
 * @param ... Аргументы
 */
void text_printf(const char *format_string, ...) {
    va_list args;

    // Ищем первый аргумент
    va_start(args, format_string);

    printf(text_putc, format_string, args);

    // Освобождаем память
    va_end(args);
}
