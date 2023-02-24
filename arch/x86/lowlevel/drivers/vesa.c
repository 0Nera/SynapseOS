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

#include <arch.h>
#include <libk.h>
#include <multiboot.h>

extern uint32_t *multiboot_framebuffer; ///< Адресс фреймбуффера полученный от загрузчика
extern uint32_t multiboot_framebuffer_width;
extern uint32_t multiboot_framebuffer_height;
extern uint32_t multiboot_framebuffer_pitch;
extern uint32_t multiboot_framebuffer_bpp;

void vesa_put_pixel(int x, int y, uint32_t color) {
    uint32_t where = x + y * (multiboot_framebuffer_pitch / 4);
    multiboot_framebuffer[where] = color;
}

void vesa_put_rect(size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color) {
    for (size_t y = y1; y < y2; y++) {
        for (size_t x = x1; x < x2; x++) {
            vesa_put_pixel(x, y, color);
        }
    }
}

void vesa_draw_background(uint32_t color) {
    vesa_put_rect(0, 0, multiboot_framebuffer_width, multiboot_framebuffer_height, color);
}

void putLine(int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int d = 2 * dy - dx;
    vesa_put_pixel(x0, y0, color);
    int y = y0;

    for (int x = x0; x <= x1; ++x) {
        vesa_put_pixel(x, y, color);
        d = d + (2 * dy);
        if (d > 0) {
            y = y + 1;
            d = d - (2 * dx);
        }
    }
}

void vesa_init(struct multiboot_info* info) {
    multiboot_framebuffer = (uint32_t*)(uintptr_t)info->framebuffer_addr;
    multiboot_framebuffer_pitch = info->framebuffer_pitch;
    multiboot_framebuffer_bpp = info->framebuffer_bpp;
    multiboot_framebuffer_width = info->framebuffer_width;
    multiboot_framebuffer_height = info->framebuffer_height;

    debug_log("vbe_mode %u", info->vbe_mode);
    debug_log("multiboot_framebuffer 0x%x", multiboot_framebuffer);
    debug_log("multiboot_framebuffer_pitch %u", multiboot_framebuffer_pitch);
    debug_log("multiboot_framebuffer_bpp %u", multiboot_framebuffer_bpp);
    debug_log("multiboot_framebuffer_width %u", multiboot_framebuffer_width);
    debug_log("multiboot_framebuffer_height %u", multiboot_framebuffer_height);
    paging_identity_map(info->framebuffer_addr, multiboot_framebuffer_width * multiboot_framebuffer_height * (multiboot_framebuffer_bpp / 8));
    memset(multiboot_framebuffer, '1', multiboot_framebuffer_width * multiboot_framebuffer_height * (multiboot_framebuffer_bpp / 8));
}