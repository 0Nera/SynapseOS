#include <vesa.h>



long abs(long l) {
    if (l < 0) {
        return -l;
    } else {
        return l;
    }
}


void draw_pixel(int x, int y, unsigned int color) {
    asm volatile("int $0x80" 
                :
                : "a"(SC_CODE_putpixel),
                  "b"(x),
                  "d"(y),
                  "c"(color)
                );
}


void draw_line(int x, int y, int x2, int y2, unsigned int color) {
    int dx, dy, step, i;
    dx = abs(x2 - x);
    dy = abs(y2 - y);
    
    if(dx>=dy){
        step=dx;
    } else {
        step=dy;
    }
    
    dx=dx/step;
    dy=dy/step;
    
    x=x;
    y=y;
    
    i=1;
    while(i<=step) {
        draw_pixel(x,y,5);
        x=x+dx;
        y=y+dy;
        i=i+1;
    }
}