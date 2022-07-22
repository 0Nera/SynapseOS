#pragma once
#include <libk/string.h>


uint32_t vga_index_db;
static uint32_t next_line_index = 1;
uint8_t g_fore_color = VESA_WHITE, g_back_color = VESA_BLUE;
uint16_t* vga_buffer;
#define VGA_ADDRESS 0xB8000
#define BUFSIZE 790000 // Размер буфера


#define BOX_SINGLELINE 1
#define BOX_DOUBLELINE 2

#define x_start 0                           // Начало по X (самый левый пиксель)
#define x_end 1024                          // Начало по X (самый правый пиксель)

#define y_start 0                           // Начало по Y (самый левый пиксель)
#define y_end 768                          // Начало по Y (самый правый пиксель)


void print_char(char ch);
void print_string(char *str);
uint32_t digit_count(uint8_t num);
void print_color_string(char *str, uint8_t fore_color, uint8_t back_color);
void print_int(uint8_t num);
uint16_t get_box_draw_char(uint8_t chn, uint8_t fore_color, uint8_t back_color);
void clear_vga_buffer(uint16_t **buffer, uint8_t fore_color, uint8_t back_color);
void gotoxy(uint16_t x, uint16_t y);
void draw_generic_box(uint16_t x, uint16_t y,
                      uint16_t width, uint16_t height,
                      uint8_t fore_color, uint8_t back_color,
                      uint8_t topleft_ch,
                      uint8_t topbottom_ch,
                      uint8_t topright_ch,
                      uint8_t leftrightside_ch,
                      uint8_t bottomleft_ch,
                      uint8_t bottomright_ch);
void draw_box(uint8_t boxtype,
              uint16_t x, uint16_t y,
              uint16_t width, uint16_t height,
              uint8_t fore_color, uint8_t back_color);

void fill_box(uint8_t ch, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
void init_vga(uint8_t fore_color, uint8_t back_color);
int tui();
