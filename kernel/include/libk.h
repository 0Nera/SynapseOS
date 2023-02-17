/**
 * @file libk.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Стандартная библиотека ядра
 * @version 0.1.0
 * @date 18-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#ifndef _LIBK_H
#define _LIBK_H

#define MODULE(name)

/**
 * @brief Используем встроенный в GCC тип для функций которые не возвращают ничего
 *
 */
#define noreturn _Noreturn

/**
 * @brief Инициализация AP до использования макросами va_arg и va_end.
 *
 */
#define va_start(v, l) __builtin_va_start(v, l)

/**
 * @brief Освобождение памяти va_list
 *
 */
#define va_end(v) __builtin_va_end(v)

/**
 * @brief Парсинг аргументов va_list
 *
 */
#define va_arg(v, l) __builtin_va_arg(v, l)

/**
 * @brief Копирование va_list
 *
 */
#define va_copy(d, s) __builtin_va_copy(d, s)

/**
 * @brief Поиск наибольшего числа
 *
 */
#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/**
 * @brief Верно
 *
 */
#define true 1

/**
 * @brief Ложь
 *
 */
#define false 0

/**
 * @brief Реализация NULL
 *
 */
#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 * @brief Атрибут для упакованных структур
 *
 */
#define PACKED __attribute__((packed))

/**
 * @brief Число от -128 до 127 (1 байт)
 *
 */
typedef char int8_t;

/**
 * @brief Число от -От -32768 до 32767 (2 байта)
 *
 */
typedef short int16_t;

/**
 * @brief Число от -2147483648 до 2147483647 (4 байта)
 *
 */
typedef int int32_t;

/**
 * @brief Число от 0 до 255 (1 байт)
 *
 */
typedef unsigned char uint8_t;

/**
 * @brief Число от 0 до 65536 (2 байта)
 *
 */
typedef unsigned short uint16_t;

/**
 * @brief Число от 0 до 4294967296 (4 байта)
 *
 */
typedef unsigned int uint32_t;

/**
 * @brief Число от 0 до 18446744073709551616 (8 байт)
 *
 */
typedef unsigned long uint64_t;

/**
 * @brief Целочисленный тип данных для измерения величин
 *
 */
typedef __SIZE_TYPE__ size_t;

/**
 * @brief Специальный тип указателей
 *
 */
typedef __UINTPTR_TYPE__ uintptr_t;

/**
 * @brief Специальный тип для булевых операций
 *
 */
typedef _Bool bool;

/**
 * @brief Список аргументов
 *
 */
typedef __builtin_va_list va_list;

uint32_t strlen(const char* string);
void memset(void* address, uint8_t value, uint32_t length);
int32_t strcmp(const char* string1, const char* string2);
void* memmove(void* dst, void* src, size_t n);
int32_t memcmp(const void* s1, const void* s2, size_t n);
char* strcpy(char* dest, const char* src);
int pow(int x, unsigned int y);
bool multiboot2_init(unsigned int addr);

#include <libk/draw.h>
#include <libk/list.h>

#endif // libk.h