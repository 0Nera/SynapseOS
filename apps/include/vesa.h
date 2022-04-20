#pragma once
#define SC_CODE_putpixel 32
#define SC_CODE_drawline 33

enum colors  {
    VESA_BLACK = 0x000000,
    VESA_BLUE = 0x0000AA,
    VESA_GREEN = 0x00AA00,
    VESA_CYAN = 0x00AAAA,
    VESA_RED = 0xAA0000,
    VESA_MAGENTA = 0xAA00AA,
    VESA_BROWN = 0xAA5500,
    VESA_LIGHT_GREY = 0xAAAAAA,
    VESA_DARK_GREY = 0x555555,
    VESA_LIGHT_BLUE = 0x5555FF,
    VESA_LIGHT_GREEN = 0x55FF55,
    VESA_LIGHT_CYAN = 0x55FFFF,
    VESA_LIGHT_RED = 0xFF5555,
    VESA_LIGHT_MAGENTA = 0xFF55FF,
    VESA_LIGHT_BROWN = 0xffff55,
    VESA_WHITE = 0xFFFFFF,
};

void draw_pixel(int x, int y, unsigned int color) {
    unsigned int arguments[3];
    arguments[0] = x;
    arguments[1] = y;
    arguments[2] = color;

    unsigned int res = 0;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_putpixel), "b"(arguments));
    asm volatile("int $0x80");
}

void draw_line(int x, int y, int x1, int y2, unsigned int color) {
    unsigned int arguments[5];
    arguments[0] = x;
    arguments[1] = y;
    arguments[2] = x1;
    arguments[3] = y2;
    arguments[4] = color;

    unsigned int res = 0;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_putpixel), "b"(arguments));
    asm volatile("int $0x80");
}