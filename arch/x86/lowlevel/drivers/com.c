/**
 * @file com.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции для работы с com1 портом
 * @version 0.1.0
 * @date 19-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <arch.h>
#include <drivers/com.h>
#include <libk.h>
#include <print.h>
#include <tools.h>

extern bool pit_lock;

/**
 * @brief Проверка "занятости" COM1 порта
 *
 * @return int Количество задач
 */
static int com1_log_is_transmit_empty() {
    return ports_inb(PORTS_COM1 + 5) & 0x20;
}

/**
 * @brief Вывод символа в COM1 порт
 *
 * @param c Выводимый символ
 */
static void com1_log_putchar(char c) {
    pit_lock = true;
    while (com1_log_is_transmit_empty() == 0)
        ;
    ports_outb(PORTS_COM1, c);
    pit_lock = false;
}

/**
 * @brief Вывод в COM1 порт форматированной строки используя неопределенное количество аргументов
 *
 * @param format_string Строка форматов
 * @param ... Аргументы
 */
void com1_log_printf(const char *format_string, ...) {
    va_list args;

    // Ищем первый аргумент
    va_start(args, format_string);

    printf(com1_log_putchar, format_string, args);

    // Освобождаем память
    va_end(args);
}