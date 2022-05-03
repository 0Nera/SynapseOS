#include <vesa.h>


#define x_start 0
#define x_end 1024


int main(){
    for (int x = x_start; x != x_end; x++){
        for (int y = 0; y != 64; y++){
            draw_pixel(x, y, VESA_WHITE);
        }
    }

    for (int x = x_start; x != x_end; x++){
        for (int y = 64; y != 128; y++){
            draw_pixel(x, y, VESA_BLUE);
        }
    }

    for (int x = x_start; x != x_end; x++){
        for (int y = 128; y != 192; y++){
            draw_pixel(x, y, VESA_RED);
        }
    }

    return 0;
}