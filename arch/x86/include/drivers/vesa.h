/**
 * @file vesa.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Драйвер VESA
 * @version 0.1.0
 * @date 06-02-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */


#ifndef _DRIVER_VESA_H
#define _DRIVER_VESA_H


#include <multiboot.h>


void vesa_init(struct multiboot_info* info);
void vesa_put_pixel(int x, int y, uint32_t color);


#endif // vesa.h