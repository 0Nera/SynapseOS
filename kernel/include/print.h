#ifndef PRINT_H
#define PRINT_H

#include <libk.h>

typedef void (*putc_ptr)(char c);

/**
 * @brief Вывод форматированной строки используя неопределенное количество аргументов
 *
 * @param f Указатель на функцию вывода символа
 * @param format_string Строка форматов
 * @param ... Аргументы
 */
void printf(putc_ptr f, const char *format_string, va_list args);

#endif
