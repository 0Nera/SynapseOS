#include <vesa.h>
#include <stdint.h>
#include <ports.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i = 0;

void draw_line (int x1, int x2,int y, unsigned int colr) {
    i += x1;
    while  (i < x2)
    {
        draw_pixel(i, y, colr);
        i++;
    }
}
int main () {
    draw_line(25, 150, 25, VESA_GREEN);
}