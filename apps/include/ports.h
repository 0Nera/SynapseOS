#include <stdint.h>
#include <string.h>

#define PORT_COM1 0x3f8


void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
int com1_is_transmit_empty();
void com1_write_char(char a);
void qemu_putstring(char log_data[]);
void io_wait();
void sleep(int i);