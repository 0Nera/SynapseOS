#include <vesa.h>

/**
 * @brief Очистка экрана
 * 
 */
void clean_screen(){
    for (int32_t x = 0; x < VESA_WIDTH; x++){
        for (int32_t y = 0; y < VESA_HEIGHT; y++){
            set_pixel(x, y, VESA_BLACK);
        }
    }

    tty_pos_x = 0;
    tty_pos_y = -17;

    qemu_log("Screan cleaned!");
}



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
