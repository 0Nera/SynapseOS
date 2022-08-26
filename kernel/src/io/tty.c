/**
 * @file tty.c
 * @authors Арен Елчинян (a2.dev@yandex.com), <>
 * @brief Прослойка для работы с текстовым терминалом и графикой VESA
 * @version 0.2.0
 * @date 2022-08-08
 * 
 * @copyright Copyright Арен Елчинян (c) 2022
 * 
 */

#define EXPERIMENTAL_FONT

#ifdef  EXPERIMENTAL_FONT
#include "../fonts/font_Experimental_Font.h"
#endif

#define ENABLE_DOUBLE_BUFFERING 1

#include <kernel.h>
uint8_t *framebuffer_addr;
uint32_t framebuffer_pitch;
uint32_t framebuffer_bpp;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint32_t framebuffer_size;

uint8_t *back_framebuffer_addr;

volatile uint8_t tty_feedback = 1;

size_t tty_line_fill[1024];

int32_t tty_pos_x;
int32_t tty_pos_y;

uint32_t tty_text_color;

SynapseTTYInfo* get_tty_info() {
    SynapseTTYInfo* ty;
    ty->width = framebuffer_width;
    ty->height = framebuffer_height;
    ty->x = tty_pos_x;
    ty->y = tty_pos_y;
    return ty;
}

void punch() {
    #if ENABLE_DOUBLE_BUFFERING==1
    memcpy(framebuffer_addr, back_framebuffer_addr, framebuffer_size);
    #endif
}

/**
 * @brief Слияние символа и цвета, для вывода
 * 
 */
uint16_t vga_entry(uint8_t c, uint8_t tty_color) {
    return (uint16_t) c | (uint16_t) tty_color << 8;
}


/**
 * @brief Получение позиции по x
 * 
 * @return int32_t - позиция по x
 */
int32_t getPosX(){
    return tty_pos_x;
}


/**
 * @brief Получение позиции по y
 * 
 * @return int32_t - позиция по y
 */
int32_t getPosY(){
    return tty_pos_y;
}


/**
 * @brief Изменение цвета текста
 * 
 * @param color - цвет
 */
void tty_setcolor(int32_t color) {
    tty_text_color = color;
}


/**
 * @brief Инициализация графики
 * 
 * @param mboot - информация полученная от загрузчика
 */
void init_vbe(multiboot_info *mboot) {
    svga_mode_info_t *svga_mode = (svga_mode_info_t*) mboot->vbe_mode_info;
    framebuffer_addr = (uint8_t *)svga_mode->physbase; 
    framebuffer_pitch = svga_mode->pitch;
    framebuffer_bpp = svga_mode->bpp;
    framebuffer_width = svga_mode->screen_width;
    framebuffer_height = svga_mode->screen_height;
    framebuffer_size = framebuffer_height * framebuffer_pitch;

    physical_addres_t frame;
    virtual_addr_t virt;

    for (frame = (physical_addres_t)framebuffer_addr, virt = (virtual_addr_t)framebuffer_addr;
        frame < (physical_addres_t)(framebuffer_addr + framebuffer_size);
        frame += PAGE_SIZE, virt += PAGE_SIZE) {
        vmm_map_page(frame, virt);
    }
   qemu_log("VBE create_back_framebuffer");

   create_back_framebuffer(); // PAGE FAULT CAUSES HERE!!! 
   qemu_log("^---- OKAY");
}


/**
 * @brief Создание второго буффера экрана
 * 
 */
void create_back_framebuffer() {
	qemu_log("^---- 1. Allcoating"); // Я не знаю почему, но это предотвратило падение, но устроило его в другом месте
    back_framebuffer_addr = kheap_malloc(framebuffer_size);

    qemu_log("back_framebuffer_addr = %x", back_framebuffer_addr);
    memset(back_framebuffer_addr, 0, framebuffer_size); // Должно предотвратить падение
}


/**
 * @brief Очистка экрана и сброс настроек
 * 
 * @param mboot_info - информация о дисплее от загрузчика
 */
void tty_init(struct multiboot_info *mboot_info) {
    tty_pos_y = 0;
    tty_pos_x = 0;

    tty_text_color = COLOR_SYS_TEXT;

    svga_mode_info_t *svga_mode = (svga_mode_info_t*) mboot_info->vbe_mode_info;

    framebuffer_addr = (uint8_t *)svga_mode->physbase;
    framebuffer_pitch = svga_mode->pitch;
    framebuffer_bpp = svga_mode->bpp;
    framebuffer_width = svga_mode->screen_width;
    framebuffer_height = svga_mode->screen_height;
    framebuffer_size = framebuffer_height * framebuffer_pitch;
    back_framebuffer_addr = framebuffer_addr;
    tty_printf("[Display] %dx%d@%d\n",framebuffer_width,framebuffer_height,framebuffer_pitch);
}


/**
 * @brief Прокрутка экрана на 1 строку
 * 
 */
void tty_scroll() {
    uint32_t num_rows = 1;
    tty_pos_y -= 17*num_rows;

    // Копируем строки сверху
    uint8_t *read_ptr = (uint8_t*) back_framebuffer_addr + ((num_rows * 17) * framebuffer_pitch);
    uint8_t *write_ptr = (uint8_t*) back_framebuffer_addr;
    uint32_t num_bytes = (framebuffer_pitch * VESA_HEIGHT) - (framebuffer_pitch * (num_rows * 17));
    memcpy(write_ptr, read_ptr, num_bytes);

    // Очистка строк
    write_ptr = (uint8_t*) back_framebuffer_addr + (framebuffer_pitch * VESA_HEIGHT) - (framebuffer_pitch * (num_rows * 17));
    memset(write_ptr, 0, framebuffer_pitch * (num_rows * 17));

    // Копируем буфферы
    #if ENABLE_DOUBLE_BUFFERING==0
    memcpy(framebuffer_addr, back_framebuffer_addr, framebuffer_size);
    #endif
}


/**
 * @brief Вывод одного пикселя на экран
 * 
 * @param x - позиция по x
 * @param y - позиция по y
 * @param color - цвет
 */
void set_pixel(int32_t x, int32_t y, uint32_t color) {
    if (x < 0 || y < 0 || 
        x >= (int) VESA_WIDTH || 
        y >= (int) VESA_HEIGHT) {
        return;
    }

    unsigned where = x * (framebuffer_bpp / 8) + y * framebuffer_pitch;

    #if ENABLE_DOUBLE_BUFFERING==0
    framebuffer_addr[where] = color;
    framebuffer_addr[where + 1] = color >> 8;
    framebuffer_addr[where + 2] = color >> 16;
    #else
    back_framebuffer_addr[where] = color & 255;
    back_framebuffer_addr[where + 1] = (color >> 8) & 255;
    back_framebuffer_addr[where + 2] = (color >> 16) & 255;
    #endif
}


/**
 * @brief Изменяем позицию курсора по X
 * 
 * @param x - позиция по X
 */
void setPosX(int32_t x){
    tty_pos_x = x;
}


/**
 * @brief Изменяем позицию курсора по Y
 * 
 * @param y - позиция по Y
 */
void setPosY(int32_t y){
    tty_pos_y = y;
}


/**
 * @brief Получение длины экрана
 * 
 * @return uint32_t - длина
 */
uint32_t getWidthScreen(){
    return framebuffer_width;
}


/**
 * @brief Получение ширины экрана
 * 
 * @return uint32_t - ширина
 */
uint32_t getHeightScreen(){
    return framebuffer_height;
}


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


/**
 * @brief Вывод линии на экран
 * 
 * @param x - координата точки 1 по x
 * @param y - координата точки 1 по y
 * @param xe - координата точки 2 по x
 * @param ye - координата точки 2 по y
 * @param color - цвет
 */
void set_line(int32_t x, int32_t y, int32_t xe, int32_t ye, uint32_t color){
    for (int32_t i = x; i < xe; i++) {
        for (int32_t j = y; j < ye; j++) {
            set_pixel(i, j, color);
        }
    }
}

/**
 * @brief Рисуем прямоугольник
 *
 * @param x - Начальная координата X
 * @param y - Начальная координата y
 * @param w - Длина
 * @param h - Высота
 * @param color - цвет заливки
 */
void drawRect(int x,int y,int w, int h,int color){
    for (int _x = x; _x < x+w ; _x++){
        for (int _y = y; _y < y+h; _y++){
            set_pixel(_x, _y, color);
        }
    }
}


/**
 * @brief Вывод одного символа с учетом цвета фона и текста
 * 
 * @param c - символ
 * @param txColor - цвет текста
 * @param bgColor - цвет фона
 */
void _tty_putchar_color(char c, uint32_t txColor, uint32_t bgColor) {

    if ((tty_pos_x + 8) >= (int)VESA_WIDTH || c == '\n') {
        tty_line_fill[tty_pos_y] = tty_pos_x;
        tty_pos_x = 0;

        if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) {
            tty_scroll();
        } else {
            tty_pos_y += 17;
        }
    } else {

        if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) {
            tty_scroll();
        }
        draw_vga_character(c, tty_pos_x, tty_pos_y, txColor, bgColor, 1);
        tty_pos_x += 8;
    }
}

void tty_putchar_color(char c, uint32_t txColor, uint32_t bgColor) {
    _tty_putchar_color(c, txColor, bgColor);
    punch();
}

/**
 * @brief Вывод одного символа
 * 
 * @param c - символ
 */
void _tty_putchar(char c) {
    if ((tty_pos_x + 8) >= (int)VESA_WIDTH || c == '\n') { 
        tty_line_fill[tty_pos_y] = tty_pos_x;
        tty_pos_x = 0;

        if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) { 
            tty_scroll();
        } else {
            tty_pos_y += 17;
        }
    } else {

        if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) {
            tty_scroll();
        }
        draw_vga_character(c, tty_pos_x, tty_pos_y, tty_text_color, 0x000000, 0);
        tty_pos_x += 8;
    }
}

void tty_putchar(char c) {
    _tty_putchar(c);
    punch();
}


/**
 * @brief Вывод символа на экран с учетом позиции, цвета текста и фона
 * 
 * @param c - символ
 * @param x - позиция по x
 * @param y - позиция по y
 * @param fg - цвет текста
 * @param bg - цвет фона
 * @param bgon - поменять ли местами цвета
 */
void draw_vga_character(uint8_t c, int32_t x, int32_t y, int32_t fg, int32_t bg, bool bgon) {

    int32_t cx, cy;
    int32_t mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    unsigned char *glyph = (uint8_t*) vgafnt + (int) c * 16;

    for (cy = 0; cy < 16; cy++) {
        for (cx = 0; cx < 8; cx++) {
            if (glyph[cy] & mask[cx]) {
                set_pixel(x + cx, y + cy, fg);
            } else if (bgon == true) {
                set_pixel(x + cx, y + cy, bg);
            }
        }
    }
}

void external_draw_grapheme(int* glyphs, int width, int height, unsigned char grapheme) {
    uint32_t cx, cy;
    unsigned int glyph_len = width*height;

    qemu_log("GOT TO GRAPHEMES! Grapheme: %d", grapheme);
    qemu_log("Len: %d, Position: %d", glyph_len, glyph_len*grapheme);

    for(cy = 0; cy<height; cy++) {
        for(cx = 0; cx<width; cx++) {
            int px = cx + cy*width;
            set_pixel(cx+tty_pos_x, cy+tty_pos_y, glyphs[glyph_len*grapheme + px]);
        }
    }

    tty_pos_x += width;
}

/**
 * @brief Удаление последнего символа
 * 
 */
void tty_backspace() {
    if (tty_pos_x < 8) { // Old: == 0
        if (tty_pos_y >= 17) {
            tty_pos_y -= 17;
        }
        tty_pos_x = tty_line_fill[tty_pos_y];
    } else {
        tty_pos_x -= 8;
    }
    draw_vga_character(' ', tty_pos_x, tty_pos_y, tty_text_color, 0x000000, 1);
    punch();
}


/**
 * @brief Вывод строки
 * 
 * @param str - строка
 */
void _tty_puts(const char str[]) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == '\033') {
            i++;
            if(str[i] =='[') {
                char* num = kheap_malloc(4);
                char idx = 0;
                i++;
                while(true) {
                    if(str[i]=='m') break;
                    num[idx] = str[i];
                    idx++;
                    i++;
                }
                i++;
                // tty_puts("<");
                // tty_puts(num);
                // tty_puts(">");
                if(strcmp(num, "31")==0) {
                    tty_setcolor(VESA_RED);
                } else if(strcmp(num, "32")==0) {
                    tty_setcolor(VESA_GREEN);
                } else if(strcmp(num, "33")==0) {
                    tty_setcolor(VESA_LIGHT_YELLOW);
                } else if(strcmp(num, "34")==0) {
                    tty_setcolor(VESA_BLUE);
                } else if(strcmp(num, "35")==0) {
                    tty_setcolor(VESA_MAGENTA);
                } else if(strcmp(num, "36")==0) {
                    tty_setcolor(VESA_CYAN);
                } else if(strcmp(num, "37")==0) {
                    tty_setcolor(VESA_WHITE);
                }
                else if(strncmp(num, "0", 1)==0) {
                    tty_setcolor(COLOR_SYS_TEXT);
                }
                kheap_free(num);
            }
        }
        #ifdef EXPERIMENTAL_FONT

        if(str[i]=='\xFF' && i+1<=strlen(str)) {
            char extch = str[i+1];
            external_draw_grapheme(Experimental_Font_data, Experimental_Font_width,
                                   Experimental_Font_height, extch);
            i++;
        }else{
            _tty_putchar(str[i]);
        }
        #else
        _tty_putchar(str[i]);
        #endif
    }
}

void tty_puts(const char str[]) {
    _tty_puts(str);
    punch();
}

void external_draw_grapheme_override_black(int* glyphs, int width, int height, unsigned char grapheme, int bgColor) {
    uint32_t cx, cy;
    unsigned int glyph_len = width*height;

    for(cy = 0; cy<height; cy++) {
        for(cx = 0; cx<width; cx++) {
            int px = cx + cy*width;
            if(glyphs[glyph_len*grapheme + px]!=0){
                set_pixel(cx+tty_pos_x, cy+tty_pos_y, glyphs[glyph_len*grapheme + px]);
            }else{
                set_pixel(cx+tty_pos_x, cy+tty_pos_y, bgColor);
            }
        }
    }

    tty_pos_x += width;
}

/**
 * @brief Вывод цветного текста
 * 
 * @param str - текст
 * @param txColor - цвет текста
 * @param bgColor - цвет фона
 */
void _tty_puts_color(const char str[], uint32_t txColor, uint32_t bgColor) {
    for (size_t i = 0; i < strlen(str); i++) {
        #ifdef EXPERIMENTAL_FONT
        if(str[i]=='\xFF' && i+1<=strlen(str)) {
            char extch = str[i+1];
            external_draw_grapheme_override_black(Experimental_Font_data, Experimental_Font_width,
                                   Experimental_Font_height, extch, bgColor);
            i++;
        }else{
            _tty_putchar_color(str[i], txColor, bgColor);
        }
    #else
        _tty_putchar_color(str[i], txColor, bgColor);
    #endif
    }
}

void tty_puts_color(const char str[], uint32_t txColor, uint32_t bgColor) {
    _tty_puts_color(str, txColor, bgColor);
    punch();
}


/**
 * @brief Вывод целого числа
 * 
 * @param i - число
 */
void _tty_putint(const int32_t i) {
    char res[32];
    
    if (i < 0) {
        //tty_putchar('-');
    }

    itoa(i, res);
    _tty_puts(res);
}

void tty_putint(const int32_t i) {
    _tty_putint(i);
    punch();
}

/**
 * @brief Вывод HEX числа
 * 
 * @param i - число
 */
void _tty_puthex(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;

    _tty_puts("0x");

    while((i / d == 0) && (d >= 0x10)) d /= 0x10;

    n = i;

    while( d >= 0xF ) {
        _tty_putchar(hex[n/d]);
        n = n % d;
        d /= 0x10;
    }

    _tty_putchar(hex[n]);
}

void tty_puthex(uint32_t i) {
    _tty_puthex(i);
    punch();
}


/**
 * @brief Вывод числа HEX без префикса 0x
 * 
 * @param i - число
 */
void _tty_puthex_v(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;

    while((i / d == 0) && (d >= 0x10)) d /= 0x10;

    n = i;

    while( d >= 0xF ) {
        tty_putchar(hex[n/d]);
        n = n % d;
        d /= 0x10;
    }

    tty_putchar(hex[n]);
}

void tty_puthex_v(uint32_t i) {
    _tty_puthex_v(i);
    punch();
}


/**
 * @brief Подфункция-обработчик для tty_printf
 * 
 * @param format - строка форматов
 * @param args - аргументы
 */
void _tty_print(char *format, va_list args) {
    int32_t i = 0;

    while (format[i]) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's':
                    _tty_puts(va_arg(args, char*));
                    break;
                case 'c':
                    _tty_putchar(va_arg(args, int));
                    break;
                case 'f':
                    _tty_putchar(va_arg(args, float));
                    break;
                case 'd':
                    _tty_putint(va_arg(args, int));
                    break;
                case 'i':
                    _tty_putint(va_arg(args, int));
                    break;
                case 'u':
                    _tty_putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    _tty_puthex(va_arg(args, uint32_t));
                    break;
                case 'v':
                    _tty_puthex_v(va_arg(args, uint32_t));
                    break;
                default:
                    _tty_putchar(format[i]);
            }
            // \n
        } else if (format[i] == 10) {
            tty_line_fill[tty_pos_y] = tty_pos_x;
            tty_pos_x = 0;

            if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) { 
                tty_scroll();
            } else {
                tty_pos_y += 17;
            }
            // \t
        } else if (format[i] == 9) {
            tty_pos_x += 4 * 17;
        } else {
            _tty_putchar(format[i]);
        }
        i++;
    }
}

void tty_print(char *format, va_list args) {
    _tty_print(format, args);
    punch();
}


/**
 * @brief Вывод форматированной строки на экран (аналог printf)
 * 
 * @param text - строка форматов
 * @param ... - параметры
 */
void tty_printf(char *text, ...) {
    va_list args;
    va_start(args, text);
    tty_print(text, args);
<<<<<<< HEAD
}
=======
    va_end(args);
}
>>>>>>> f6650a3cc7e61bd79b540e8fba5f9416b314ea0d
