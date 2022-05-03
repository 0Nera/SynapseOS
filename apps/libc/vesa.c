#include <vesa.h>


void draw_pixel(int x, int y, unsigned int color) {
    void *res = 0;
    unsigned int arguments[3];

    arguments[0] = x;
    arguments[1] = y;
    arguments[2] = color;


    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_putpixel), "b"(arguments));
    asm volatile("int $0x80");
}