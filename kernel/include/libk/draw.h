/**
 * @file draw.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции для рисования
 * @version 0.1.0
 * @date 02-12-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <libk.h>

#ifndef _LIBK_DRAW_H
#define _LIBK_DRAW_H 1

/**
 * @brief Структура холста
 *
 */
typedef struct {
    uint32_t width; ///< Ширина холста
    uint32_t height; ///< Высота холста
    uint32_t pitch; ///< Байт на вертикальную линию шириной в 1 пиксель от конца до начала экрана
    uint32_t bpp; ///< Байт на пиксель
    uint8_t* framebuffer; ///< Адрес видеобуффера холста
} canvas_t;

/**
 * @brief  Вывод форматированной строки
 *
 * @param format_string Форматированная строка
 * @param length Длина строки
 * @param x Позиция по X
 * @param y Позиция по Y
 * @param ... Аргументы
 */
void text_printf(const char* format_string, ...);

bool text_init();

#include <libk/draw/psf.h>
#include <libk/draw/text.h>

#endif // draw.h