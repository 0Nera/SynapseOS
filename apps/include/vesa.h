#pragma once

#include <stdint.h>

typedef struct SynapseTTYInfo {
    int x;
    int y;
    int width;
    int height;
} SynapseTTYInfo;

#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_getttyinfo      34
#define SC_CODE_version         40

#define SC_CODE_TTY_POS_X       45
#define SC_CODE_TTY_POS_Y       46
#define SC_CODE_TTY_SCREEN_W    47
#define SC_CODE_TTY_SCREEN_H    48
#define SC_CODE_TTY_SET_COLOR   49
#define SC_CODE_TTY_SET_POS_X   50
#define SC_CODE_TTY_SET_POS_Y   51
#define SC_CODE_TTY_PUT_COLOR   52
#define SC_CODE_TTY_PUTS_COLOR  53
#define SC_CODE_TTY_DRAW_VGA    54
#define SC_CODE_TTY_DRAW_RECT   57

#define SC_CODE_punch         	72

// Фон
#define COLOR_BACK  0x000000
// Текст
#define COLOR_TEXT  0xFFFFFF

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
    VESA_LIGHT_YELLOW = 0xffff55,
    VESA_WHITE = 0xFFFFFF,
};

void draw_pixel(int x, int y, unsigned int color);
void clean_screen();
SynapseTTYInfo* get_tty_info();
void punch();