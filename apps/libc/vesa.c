/**
 * @file apps/vesa.c
 * @authors Арен Елчинян (a2.dev@yandex.com), Пиминов Никита (github.com/pimnik98 | VK: @piminov_remont)
 * @brief Прослойка для работы с текстовым терминалом и графикой VESA
 * @version 0.0.2
 * @date 2022-08-23
 *
 * @copyright Copyright Арен Елчинян (c) 2022 / Пиминов Никита (с) 2022
 *
 */
#include <vesa.h>

int32_t bgVesaColor = COLOR_BACK;       // Фон на экране
int32_t txVesaColor = COLOR_TEXT;       // Основной текст для печати на экране

SynapseTTYInfo* get_tty_info() {
    SynapseTTYInfo* ty;
        asm volatile("int $0x80" 
                :"=a"(ty) : "a"(SC_CODE_getttyinfo));
    return ty;
}

/**
 * @brief Получение длины экрана
 *
 * @return uint32_t - длина
 */
uint32_t getWidthScreen(){
    uint32_t data;
    asm volatile("int $0x80"
                :"=a"(data) : "a"(SC_CODE_TTY_SCREEN_W));
    return data;
}


/**
 * @brief Получение ширины экрана
 *
 * @return uint32_t - ширина
 */
uint32_t getHeightScreen(){
    uint32_t data;
    asm volatile("int $0x80"
                :"=a"(data) : "a"(SC_CODE_TTY_SCREEN_H));
    return data;
}



/**
 * @brief Вычисляет абсолютную величину (модуль) значения
 *
 * @return long - Абсолютная величина
 */
long abs(long l) {
    if (l < 0) {
        return -l;
    } else {
        return l;
    }
}
/**
 * @brief Получение позиции курсора текста по x
 *
 * @return int32_t - позиция по x
 */
int32_t getPosX(){
    int32_t data;
    asm volatile("int $0x80"
                :"=a"(data) : "a"(SC_CODE_TTY_POS_X));
    return data;
}

/**
 * @brief Получение позиции курсора текста по y
 *
 * @return int32_t - позиция по y
 */
int32_t getPosY(){
    int32_t data;
    asm volatile("int $0x80"
                :"=a"(data) : "a"(SC_CODE_TTY_POS_Y));
    return data;
}

/**
 * @brief Вывод одного пикселя на экран
 *
 * @param x - позиция по x
 * @param y - позиция по y
 * @param color - цвет
 */

void draw_pixel(int x, int y, unsigned int color) {
    asm volatile("int $0x80" 
                :
                : "a"(SC_CODE_putpixel),
                  "b"(x),
                  "d"(y),
                  "c"(color)
                );
}

/**
 * @brief Рисуем прямоугольник (Старый метод, медленне)
 *
 * @param x - Начальная координата X
 * @param y - Начальная координата y
 * @param w - Длина
 * @param h - Высота
 * @param color - цвет заливки
 */
void drawRectOLD(int x,int y,int w, int h,int color){
    for (int _x = x; _x < x+w ; _x++){
        for (int _y = y; _y < y+h; _y++){
            draw_pixel(_x, _y, color);
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
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_DRAW_RECT),
                  "b"(x),
                  "d"(y),
                  "c"(w),
                  "S"(h),
                  "D"(color)
                );
}

/**
 * @brief Очистка экрана
 *
 */
void cleanScreen(){
    drawRect(0,0,getWidthScreen(), getHeightScreen(),bgVesaColor);
}

void punch() {
    asm volatile("int $0x80" :: "a"(SC_CODE_punch));
}

/**
 * @brief Вывод одного символа с учетом цвета фона и текста
 *
 * @param c - символ
 * @param txColor - цвет текста
 * @param bgColor - цвет фона
 */
void put_color(char c, uint32_t txColor, uint32_t bgColor) {
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_PUT_COLOR),
                  "b"(c),
                  "d"(txColor),
                  "c"(bgColor)
                );
}

/**
 * @brief Вывод одного символа с учетом цвета фона и текста
 *
 * @param c - символ
 * @param txColor - цвет текста
 * @param bgColor - цвет фона
 */
void puts_color(const char str[], uint32_t txColor, uint32_t bgColor) {
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_PUTS_COLOR),
                  "b"(str),
                  "d"(txColor),
                  "c"(bgColor)
                );
}

/**
 * @brief Вывод символа на экран с учетом позиции, цвета текста и фона
 *
 * @param c - символ
 * @param x - позиция по x
 * @param y - позиция по y
 * @param fg - цвет текста
 * @param bg - цвет фона
 */
void draw_vga_character(uint8_t c, int32_t x, int32_t y, int32_t fg, int32_t bg) {
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_DRAW_VGA),
                  "b"(c),
                  "d"(x),
                  "c"(y),
                  "S"(fg),
                  "D"(bg)
                );
}

/**
 * @brief Рисует линию (На 16 пикселей)
 *
 * @param y - Начальная координата y
 * @param color - Цвет линии
*/
void drawLine(int y,int color){
    drawRect(0,y,getWidthScreen(), y*16,color);
}

/**
 * @brief Изменяем позицию курсора по X
 *
 * @param x - позиция по X
 */
void setPosX(int32_t x){
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_SET_POS_X),
                  "b"(x)
                );
}


/**
 * @brief Изменяем позицию курсора по Y
 *
 * @param y - позиция по Y
 */
void setPosY(int32_t y){
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_SET_POS_Y),
                  "b"(y)
                );
}

/**
 * @brief Изменение цвета текста
 *
 * @param color - цвет
 */
void setColor(int32_t color) {
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_TTY_SET_COLOR),
                  "b"(color)
                );
}

/**
 * @brief Рисует узор вокруг прямоугольника
 *
 * @param x - Начальная координата X
 * @param y - Начальная координата y
 * @param w - Длина
 * @param h - Высота
 * @param color - Цвет 1
 * @param color2 - Цвет 2
 * @param с - Символ
*/
void drawRectLine(int x,int y,int w, int h,int color,int color2, int c){
    for (int _x = x; _x < x+w ; _x += 8){
        draw_vga_character(c, _x, y, color, color2);
        draw_vga_character(c, _x, y+h-16, color, color2);
    }
    for (int _y = y; _y < y+h; _y += 16){
        draw_vga_character(c, x, _y, color, color2);
        draw_vga_character(c, x+w-8, _y, color, color2);
    }
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
