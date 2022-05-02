#pragma once

#include <stdint.h>

#define PORT_COM1 0x3f8


uint8_t inb(uint16_t port); // Чтение байта из порта
uint16_t ins(uint16_t _port);
int com1_is_transmit_empty();
void outb(uint16_t port, uint8_t  val); // Запись байта в порт
void com1_write_char(char a);
void qemu(char log_data[]);
void qemu_printf(char *text, ...);
void reboot();
void io_wait();