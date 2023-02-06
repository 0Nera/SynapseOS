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


#include <com1_log.h>
#include <libk.h>
#include <ports.h>
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
    for (uint32_t i = 0; i < strlen(string); i++) {
        com1_log_putchar(string[i]);
    }
}


/**
 * @brief Вывод шестнадцатеричного числа в COM1 порт
 * 
 * @param num Выводимое число
 */
void com1_log_printudec(int32_t i) {
    uint32_t n, d = 1000000000;
    char str[255];
    uint32_t dec_index = 0;

    while ((i / d == 0) && (d >= 10)) {
        d /= 10;
    }
    n = i;

    while (d >= 10) {
        str[dec_index++] = ((char) ((int) '0' + n / d));
        n = n % d;
        d /= 10;
    }

    str[dec_index++] = ((char) ((int) '0' + n));
    str[dec_index] = 0;
    com1_log_puts(str);
}

void com1_log_printdec(int32_t i) {
    if (i >= 0) {
        com1_log_printudec(i);
    } else {
        com1_log_putchar('-');
        com1_log_printudec(-i);
    }
}


/**
 * @brief 
 * 
 * @param i 
 */
void com1_log_printhex(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;

    com1_log_puts("0x");

    while ((i / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = i;

    while (d >= 0xF) {
        com1_log_putchar(hex[n / d]);
        n = n % d;
        d /= 0x10;
    }
    com1_log_putchar(hex[n]);
}


/**
 * @brief 
 * 
 * @param i 
 */
void com1_printhex_v(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;


    while ((i / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = i;

    while (d >= 0xF) {
        com1_log_putchar(hex[n / d]);
        n = n % d;
        d /= 0x10;
    }
    com1_log_putchar(hex[n]);
}


/**
 * @brief 
 * 
 * @param address 
 * @param size 
 */
void com1_log_dump(void *address, uint32_t size) {
    /* Преобразуем указатель в массив байт */
    uint8_t *dump = (uint8_t*) address; 
    /* Преобразуем указатель просто в число */  
    uint32_t addr_tmp = (uint32_t) address;
    /* Вспомогательные переменные */
    uint32_t mark = 0;
    
    for (uint32_t i = 0; i < size; i++) {
    
        if (mark == 0) {
            com1_log_printhex(addr_tmp);
            com1_log_puts(": ");
        }
    
        com1_log_printhex(dump[i]);
    
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
        return x*com1_log_powk(x, y/2)*com1_log_powk(x, y/2);
 
}
 

static void com1_log_printfloat(double num, int after_point) {
    int int_float = (int) num;
    com1_log_printdec(int_float);
    com1_log_putchar('.');
    
    int mut_ab = com1_log_powk(10,after_point);
    int after_point_float = (int) (num * mut_ab); //33314
    
    int multiplier = com1_log_powk(10,(after_point-1));
    //printf("MULTIPLIER : %d\n" , multiplier);
    for(int xy=0; xy<after_point; xy++) {
        int muted = (after_point_float/multiplier);
        //printf("%d\n",muted);
        muted = muted % 10;
        com1_log_printdec(muted);
        
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
    while (*format_string != '\0') {
        if (*format_string == '%') {
            format_string++;
            if (*format_string == 'x') {
                com1_log_printhex(va_arg(args, unsigned long long));
            } else if (*format_string == '8') {
                com1_log_printdec((unsigned char)va_arg(args, char*)[0]);
            } else if (*format_string == 'd') {
                com1_log_printdec(va_arg(args, int));
            } else if (*format_string == 'l') {
                com1_log_printdec(va_arg(args, long));
            } else if (*format_string == 'f') {
                com1_log_printfloat(va_arg(args, double), 7);
            } else if (*format_string == 'u') {
                com1_log_printudec(va_arg(args, unsigned int));
            } else if (*format_string == 'z') {
                com1_log_printudec(va_arg(args, size_t));
            } else if (*format_string == 's') {
                com1_log_puts(va_arg(args, char*));
            } else if (*format_string == 'c') {
                com1_log_putchar((char)va_arg(args, int));
            }
        } else {
            com1_log_putchar(*format_string);
        }
        format_string++;
    }
    
    // Освобождаем память
    va_end(args);
}