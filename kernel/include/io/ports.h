#pragma once

#include <stdint.h>

#define PORT_COM1 0x3f8
#define PORT_COM2 0x2F8
#define PORT_COM3 0x3E8
#define PORT_COM4 0x2E8
#define PORT_COM5 0x5F8
#define PORT_COM6 0x4F8
#define PORT_COM7 0x5E8
#define PORT_COM8 0x4E8


#define log(M, ...) qemu_printf("[LOG] (%s:%s:%d) " M "\n",\
        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    
uint8_t inb(uint16_t port); // Чтение байта из порта
uint16_t ins(uint16_t _port);
int32_t com1_is_transmit_empty();
void outb(uint16_t port, uint8_t  val); // Запись байта в порт
void com1_write_char(char a);
void qemu_puts(char log_data[]);
void qemu_printf(char *text, ...);
void reboot();
void io_wait();