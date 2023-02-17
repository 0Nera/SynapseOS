/**
 * @file driver.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Особые структуры для драйверов
 * @version 0.1.0
 * @date 06-02-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <arch.h>
#include <kernel.h>
#include <libk.h>

#ifndef _DRIVER_H
#define _DRIVER_H

typedef struct {
    bool (*init)(int argc, uintptr_t* argv);
    char* (*info)();
    bool (*remove)(int argc, uintptr_t* argv);
    void (*print)(const char* string, size_t length);
} driver_debug_t;

#endif // driver.h