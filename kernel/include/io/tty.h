#pragma once


#define VESA_WIDTH  framebuffer_width
#define VESA_HEIGHT framebuffer_height

typedef struct rgba_struct {
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t a;
} rgba_color;

enum colors  {
    VESA_BLACK = 0x000000,
    VESA_BLUE = 0x0000AA,
    VESA_GREEN = 0x00AA00,
    VESA_CYAN = 0x00AAAA,
    VESA_RED = 0xAA0000,
    VESA_MAGENTA = 0xAA00AA,
    VESA_YELLOW = 0xAA5500,
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

typedef struct SynapseTTYInfo {
    int x;
    int y;
    int width;
    int height;
} SynapseTTYInfo;

typedef struct svga_mode_info {
    uint16_t attributes;
    uint8_t windowA, windowB;
    uint16_t granularity;
    uint16_t windowSize;
    uint16_t segmentA, segmentB;
    uint32_t winFuncPtr;
    uint16_t pitch; 

    uint16_t screen_width, screen_height;
    uint8_t wChar, yChar, planes, bpp, banks; 
    uint8_t memoryModel, bankSize, imagePages;
    uint8_t reserved0;

    // Color masks
    uint8_t readMask, redPosition;
    uint8_t greenMask, greenPosition; 
    uint8_t blueMask, bluePosition;
    uint8_t reservedMask, reservedPosition;
    uint8_t directColorAttributes;

    uint32_t physbase;
    uint32_t offScreenMemOff;
    uint16_t offScreenMemSize;
    uint8_t reserved1[206];
} __attribute__ ((packed)) svga_mode_info_t;

extern volatile uint8_t tty_feedback;


void tty_init();
void init_vbe(multiboot_info *mboot);
void tty_backspace();
void tty_printf(char *text, ...);
void tty_putchar(char c);
void set_line(int32_t x, int32_t y, int32_t xe, int32_t ye, uint32_t color);
void set_pixel(int32_t x, int32_t y, uint32_t color);
void tty_setcolor(int32_t color);
void draw_vga_character(uint8_t c, int32_t x, int32_t y, int32_t fg, int32_t bg, bool bgon);
void create_back_framebuffer();
void setPosX(int32_t x);
void setPosY(int32_t y);
int32_t getPosX();
int32_t getPosY();
uint32_t getWidthScreen();
uint32_t getHeightScreen();
SynapseTTYInfo* get_tty_info();
void tty_puts(const char str[]);
void punch();