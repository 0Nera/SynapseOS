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


#define qemu_log(M, ...) qemu_printf("[LOG] (%s:%s:%d) " M "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    
#define assert(condition) if (condition){                 \
    qemu_log("ASSERT FAIL");                                   \
    while(1);                                             \
} 

#define unit_test(condition) if ((condition) > 0){                                     \
    qemu_printf("[TEST PASSED] (%s:%s:%d)\n", __FILE__, __FUNCTION__, __LINE__); \
} else {                                                                         \
    qemu_printf("[TEST FAILED] (%s:%s:%d)\n", __FILE__, __FUNCTION__, __LINE__); \
}

#define outw(port, val) outs(port, val)


uint8_t inb(uint16_t port); // Чтение байта из порта
void outb(uint16_t port, uint8_t  val); // Запись байта в порт

uint16_t ins(uint16_t _port);
void insl(uint16_t reg, uint32_t *buffer, int32_t quads);
void outsl(uint16_t reg, uint32_t *buffer, int32_t quads);

void outl(uint16_t port, uint32_t val);
uint32_t inl(uint16_t port);

int32_t com1_is_transmit_empty();
int is_com_port(int port);
void com1_write_char(char a);
void qemu_puts(char log_data[]);
void qemu_printf(char *text, ...);
void io_wait();
