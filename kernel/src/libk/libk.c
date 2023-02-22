/**
 * @file libk.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции для упрощения написания кода ядра
 * @version 0.1.0
 * @date 19-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <libk.h>

/**
 * @brief Адрес на функцию отладки
 *
 */
void (*debug_log_printf)(const char *format_string, ...);

/**
 * @brief Адрес на функцию отладки для ядра
 *
 */
void (*kprintf)(const char *format_string, ...);

/**
 * @brief Вычисление длины строки
 *
 * @param string Строка
 * @return uint32_t Длина строки
 */
uint32_t strlen(const char *string) {
    uint32_t length = 0;

    while (string[length]) {
        length++;
    }

    return length;
}

/**
 * @brief Заполнение памяти байтом
 *
 * @param address Адресс области
 * @param value Байт заполнения
 * @param length Длина области
 */
void memset(void *address, uint8_t value, uint32_t length) {
    uint8_t *array = (uint8_t*)address;

    for (uint32_t i = 0; i < length; i++) {
        array[i] = value;
    }
}

/**
 * @brief Проверка соответствия строк
 *
 * @param string1 Первая сравниваемая строка
 * @param string2 Вторая сравниваемая строка
 * @return int 0 - в случае успеха, 1 в случае если string1 > string2 и -1 в случае если string1 < string2
 */
int32_t strcmp(const char *string1, const char *string2) {
    int32_t s1;
    int32_t s2;

    do {
        s1 = *string1++;
        s2 = *string2++;
        if (s1 == 0)
            break;
    } while (s1 == s2);

    return (s1 < s2) ? -1 : (s1 > s2);
}

/**
 * @brief
 *
 * @param dest
 * @param src
 * @return char*
 */
char *strcpy(char *dest, const char *src) {
    do {
        *dest++ = *src++;
    } while (*src != 0);

    return dest;
}

/**
 * @brief
 *
 * @param s1
 * @param s2
 * @param n
 * @return int32_t
 */
int32_t memcmp(const void *s1, const void *s2, size_t n) {
    unsigned char u1, u2;

    for (; n--; s1++, s2++) {
        u1 = *(unsigned char*)s1;
        u2 = *(unsigned char*)s2;
        if (u1 != u2) {
            return (u1 - u2);
        }
    }
    return 0;
}

/**
 * @brief
 *
 * @param dst
 * @param src
 * @param n
 * @return void*
 */
void *memcpy(void *dst, void *src, size_t n) {
    size_t i;
    if ((uint8_t*)(dst) < (uint8_t*)(src)) {
        for (i = 0; i < n; ++i) {
            ((uint8_t*)(dst))[i] = ((uint8_t*)(src))[i];
        }
    } else {
        for (i = 0; i < n; ++i) {
            ((uint8_t*)(dst))[n - i - 1] = ((uint8_t*)(src))[n - i - 1];
        }
    }
    return dst;
}

/**
 * @brief
 *
 * @param dst
 * @param src
 * @param n
 * @return void*
 */
void *memmove(void *dst, void *src, size_t n) {
    if (dst != src) {
        memcpy(dst, src, n);
    }
    return dst;
}

/**
 * @brief
 *
 * @param i
 * @param str
 */
void itoa(size_t i, char *str) {
    uint32_t n, d = 1000000000;
    uint32_t dec_index = 0;

    while ((i / d == 0) && (d >= 10)) {
        d /= 10;
    }
    n = i;

    while (d >= 10) {
        str[dec_index++] = ((char)((int)'0' + n / d));
        n = n % d;
        d /= 10;
    }

    str[dec_index++] = ((char)((int)'0' + n));
    str[dec_index] = 0;
}

/**
 * @brief
 *
 * @param p
 * @param ch
 * @return char*
 */
char *strchr(const char *p, int ch) {
    char c;

    c = ch;
    for (;; ++p) {
        if (*p == c)
            return ((char*)p);
        if (*p == '\0')
            return (NULL);
    }
}

int pow(int x, unsigned int y) {
    if (y == 0)
        return 1;
    else if (y % 2 == 0)
        return pow(x, y / 2) * pow(x, y / 2);
    else
        return x * pow(x, y);
}
