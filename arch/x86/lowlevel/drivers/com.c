/**
 * @file com.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции для работы с com1 портом
 * @version 0.1.0
 * @date 19-10-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


#include <arch.h>
#include <drivers/com.h>
#include <libk.h>
#include <tools.h>


extern bool pit_lock;
 

/**
 * @brief Проверка "занятости" COM1 порта
 * 
 * @return int Количество задач
 */
static int com1_log_is_transmit_empty() {
    return ports_inb(PORTS_COM1 + 5) & 0x20;
}


/**
 * @brief Вывод символа в COM1 порт
 * 
 * @param c Выводимый символ
 */
static void com1_log_putchar(char c) {
    pit_lock = true;
    while (com1_log_is_transmit_empty() == 0);
    ports_outb(PORTS_COM1, c);
    pit_lock = false;
}


/**
 * @brief Вывод строки в COM1 порт
 * 
 * @param string Выводимая строка
 */
static void com1_log_puts(const char string[]) {
    while (*string)
        com1_log_putchar(*string++);
}

/**
 * @brief Вывод числа в COM1 порт
 * 
 * @param i Выводимое число
 * @param r Основание (от 2 до 16)
 */
void com1_log_printunum(uint32_t i, int32_t r) {
    const unsigned char symbols[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    char str[33] = "";
    int index = sizeof(str) - 1;

    do {
        str[--index] = symbols[i % r];
        i /= r;
    } while (i);

    com1_log_puts(str + index);
}

/**
 * @brief Вывод числа в COM1 порт
 * 
 * @param i Выводимое число
 * @param r Основание (от 2 до 16)
 */
void com1_log_printnum(int32_t i, int32_t r) {
    if (i < 0) com1_log_putchar('-');
    com1_log_printunum(-i, r);
}

/**
 * @brief 
 * 
 * @param addres 
 * @param size 
 */
void com1_log_dump(void *addres, size_t size) {
    /* Преобразуем указатель в массив байт */
    uint8_t *dump = (uint8_t*) addres; 
    /* Преобразуем указатель просто в число */  
    uint32_t addr_tmp = (uint32_t) addres;
    /* Вспомогательные переменные */
    uint32_t mark = 0;
    
    for (uint32_t i = 0; i < size; i++) {
    
        if (mark == 0) {
            com1_log_printunum(addr_tmp, 16);
            com1_log_puts(": ");
        }
    
        com1_log_printunum(dump[i], 16);
    
        switch(mark) {
            case 7:
                com1_log_putchar('|');
                mark++;
                break;
            case 16:
                com1_log_putchar('\n');
                mark = 0;
                addr_tmp += 0x10;
                break;
            default:
                com1_log_putchar(' ');
                mark++;
                break;
        }
    }
    com1_log_putchar('\n');
}



static int com1_log_powk(int x, unsigned int y) {
    if( y == 0)
        return 1;
    else if (y%2 == 0)
        return com1_log_powk(x, y/2)*com1_log_powk(x, y/2);
    else
        return x*com1_log_powk(x, y);
 
}
 

static void com1_log_printfloat(double num, int after_point) {
    int int_float = (int) num;
    com1_log_printnum(int_float, 10);
    com1_log_putchar('.');
    
    int mut_ab = com1_log_powk(10,after_point);
    int after_point_float = (int) (num * mut_ab); //33314
    
    int multiplier = com1_log_powk(10,(after_point-1));
    //printf("MULTIPLIER : %d\n" , multiplier);
    for(int xy=0; xy<after_point; xy++) {
        int muted = (after_point_float/multiplier);
        //printf("%d\n",muted);
        muted = muted % 10;
        com1_log_printnum(muted, 10);
        
        multiplier/=10;
    }
}

/**
 * @brief Вывод в COM1 порт форматированной строки используя неопределенное количество аргументов
 * 
 * @param format_string Строка форматов
 * @param ... Аргументы
 */
void com1_log_printf(const char *format_string, ...) {
    va_list args;

    // Ищем первый аргумент
    va_start(args, format_string);

    // Обработка и парсинг строки форматов
    while (*format_string) {
        if (*format_string == '%') {
            switch (*++format_string) {
            case 'x':
                com1_log_printunum(va_arg(args, uint32_t), 16);
                break;
            case '8':
                com1_log_printnum((unsigned char)va_arg(args, char*)[0], 10);
                break;
            case 'd':
                com1_log_printnum(va_arg(args, int), 10);
                break;
            case 'l':
                com1_log_printnum(va_arg(args, long), 10);
                break;
            case 'f':
                com1_log_printfloat(va_arg(args, double), 7);
                break;
            case 'u':
                com1_log_printunum(va_arg(args, unsigned int), 10);
                break;
            case 'z':
                com1_log_printunum(va_arg(args, size_t), 10);
                break;
            case 's':
                com1_log_puts(va_arg(args, char*));
                break;
            case 'c':
                com1_log_putchar((char)va_arg(args, int));
                break;
            }
        } else {
            com1_log_putchar(*format_string);
        }
        format_string++;
    }
    
    // Освобождаем память
    va_end(args);
}