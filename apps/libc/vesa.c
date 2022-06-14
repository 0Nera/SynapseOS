#include <vesa.h>


void draw_pixel(int x, int y, unsigned int color) {
    asm volatile("int $0x80" 
                :
                : "a"(SC_CODE_putpixel),
                  "b"(x),
                  "d"(y),
                  "c"(color)
                );
}