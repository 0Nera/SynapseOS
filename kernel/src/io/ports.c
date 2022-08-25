/**
 * @file ports.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Прослойка для работы с портами ввода-вывода
 * @version 0.1.0
 * @date 2022-08-08
 * @copyright Copyright Арен Елчинян (c) 2022
 */


#include <kernel.h>


/**
 * @brief Запись одного байта в порт
 * 
 * @param port - порт
 * @param val - данные
 */
void outb(uint16_t port, uint8_t val){
    asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}


/**
 * @brief Получение одного байта из порта
 * 
 * @param port - порт
 * @return uint8_t - данные
 */
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}


/**
 * @brief Запись 32х битного числа в порт
 * 
 * @param port - порт
 * @param val - число
 */
void outl(uint16_t port, uint32_t val) {
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}


/**
 * @brief Чтение 32х битного числа
 * 
 * @param port - порт
 * @return uint32_t - число
 */
uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}


/**
 * @brief Чтение word из порта
 * 
 * @param port - порт
 * @return uint16_t - word
 */
uint16_t ins(uint16_t port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}


/**
 * @brief Запись word в порт
 * 
 * @param port - порт
 * @param data - данные
 */
void outs(uint16_t port, uint16_t data) {
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}


/**
 * @brief Чтение длинного слова через порт
 * 
 * @param port - порт
 * @param buffer - данные
 * @param times - сколько данных прочесть
 */
void insl(uint16_t port, uint32_t *buffer, int32_t times) {
    for (int32_t index = 0; index < times; index++) {
        buffer[index] = inl(port);
    }
}


/**
 * @brief Запись длинного слова через порт
 * 
 * @param port - порт
 * @param buffer - данные
 * @param times - сколько данных отправить
 */
void outsl(uint16_t port, uint32_t *buffer, int32_t times) {
    for (int32_t index = 0; index < times; index++) {
        outl(port, buffer[index]);
    }
}


/**
 * @brief Проверка занятости порта COM1
 * 
 * @return int32_t - состояние
 */
int32_t com1_is_transmit_empty() {
    return inb(PORT_COM1 + 5) & 0x20;
}


/**
 * @brief Вывод одного символа через порт COM1
 * 
 * @param a 
 */
void com1_write_char(char a) {
    while (com1_is_transmit_empty() == 0);
    outb(PORT_COM1, a);
}


/**
 * @brief Вывод строки через порт COM1
 * 
 * @param log_data - строка
 */
void qemu_putstring(char log_data[]) {
    for (size_t i = 0; i < strlen(log_data); i++) {
        com1_write_char(log_data[i]);
    }
}


/**
 * @brief Небольшая задержка используя порт 128(0x80)
 * 
 */
void io_wait(void) {
    outb(0x80, 0);
}


/**
 * @brief Подфункция-обработчик для функции qemu_putint
 * 
 * @param i 
 */
void qemu_putuint(uint32_t i) {
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
    qemu_putstring(str);
}


/**
 * @brief Вывод целого числа через порт COM1
 * 
 * @param i - число
 */
void qemu_putint(int32_t i) {
    if (i >= 0) {
        qemu_putuint(i);
    } else {
        com1_write_char('-');
        qemu_putuint(-i);
    }
}


/**
 * @brief Вывод числа через порт COM1 в формате HEX
 * 
 * @param i - число
 */
void qemu_puthex(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;

    qemu_putstring("0x");

    while ((i / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = i;

    while (d >= 0xF) {
        com1_write_char(hex[n / d]);
        n = n % d;
        d /= 0x10;
    }
    com1_write_char(hex[n]);
}


/**
 * @brief Вывод числа через порт COM1 в формате HEX без 0x
 * 
 * @param i - число
 */
void qemu_puthex_v(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t n, d = 0x10000000;


    while ((i / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = i;

    while (d >= 0xF) {
        com1_write_char(hex[n / d]);
        n = n % d;
        d /= 0x10;
    }
    com1_write_char(hex[n]);
}


/**
 * @brief Подфункция-обработчик аргументов для qemu_printf
 * 
 * @param format Строка форматов
 * @param args Аргументы 
 */
void qemu_print(char *format, va_list args) {
    int32_t i = 0;
    char *string;

    while (format[i]) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
            case 's':
                string = va_arg(args, char*);
                qemu_putstring(string);
                break;
            case 'c':
                // To-Do: fix this! "warning: cast to pointer from integer of different size"
                com1_write_char((char)va_arg(args, int));
                break;
            case 'd':
                qemu_putint(va_arg(args, int));
                break;
            case 'i':
                qemu_putint(va_arg(args, int));
                break;
            case 'u':
                qemu_putuint(va_arg(args, unsigned int));
                break;
            case 'x':
                qemu_puthex(va_arg(args, uint32_t));
                break;
            case 'v':
                qemu_puthex_v(va_arg(args, uint32_t));
                break;
            default:
                com1_write_char(format[i]);
            }
        } else {
            com1_write_char(format[i]);
        }
        i++;
    }
}


/**
 * @brief Вывод через COM1 информации
 * 
 * @param text Строка с параметрами
 */
void qemu_printf(char *text, ...) {
    va_list args;
    // find the first argument
    va_start(args, text);
    // pass print32_t the output handle the format text and the first argument
    qemu_print(text, args);
    
    va_end(args);
}


/**
 * @brief Проверка, читаем ли символ
 * 
 * @param c Символ
 * @return 1 если читаемый, 0 если нет
 */
int isprint(char c) {
    return ((c >= ' ' && c <= '~') ? 1 : 0);
}


/**
 * @brief Проверка на тип порта
 * 
 * @param port 
 * @return Возвращает номер порта или 0 в случае если порт не в списке
 */
int is_com_port(int port) {
    switch (port) {
        case PORT_COM1:
            return 1;
            break;
        case PORT_COM2:
            return 2;
            break;
        case PORT_COM3:
            return 3;
            break;
        case PORT_COM4:
            return 4;
            break;
        case PORT_COM5:
            return 5;
            break;
        case PORT_COM6:
            return 6;
            break;
        case PORT_COM7:
            return 7;
            break;
        case PORT_COM8:
            return 8;
            break;
        default:
            return 0;
            break;
    }
}
