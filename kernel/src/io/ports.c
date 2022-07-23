#include <kernel.h>

void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void outl(uint16_t port, uint32_t val) {
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

uint16_t ins(uint16_t port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outs(uint16_t port, uint16_t data) {
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}

// read long word from reg port for quads times
void insl(uint16_t reg, uint32_t *buffer, int32_t quads) {
    int32_t index;
    for (index = 0; index < quads; index++) {
        buffer[index] = inl(reg);
    }
}

// write long word to reg port for quads times
void outsl(uint16_t reg, uint32_t *buffer, int32_t quads) {
    int32_t index;
    for (index = 0; index < quads; index++) {
        outl(reg, buffer[index]);
    }
}

int32_t com1_is_transmit_empty() {
    return inb(PORT_COM1 + 5) & 0x20;
}

void com1_write_char(char a) {
    while (com1_is_transmit_empty() == 0);
    outb(PORT_COM1, a);
}

void qemu_putstring(char log_data[]) {
    for (size_t i = 0; i < strlen(log_data); i++) {
        com1_write_char(log_data[i]);
    }
}

void io_wait(void) {
    outb(0x80, 0);
}


void qemu_breakpoint()  {
    log("BREAKPOINT!");
}



void qemu_putuint(int32_t i) {
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

void qemu_putint(int32_t i) {
    if (i >= 0) {
        qemu_putuint(i);
    } else {
        com1_write_char('-');
        qemu_putuint(-i);
    }
}

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

void qemu_printf(char *text, ...) {
    va_list args;
    // find the first argument
    va_start(args, text);
    // pass print32_t the output handle the format text and the first argument
    qemu_print(text, args);
}

int isprint(char c) {
    return ((c >= ' ' && c <= '~') ? 1 : 0);
}


void reboot() {
    log("REBOOT");

    uint8_t good = 0x02;

    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    asm volatile("hlt");
}


void shutdown(){
    log("SHUTDOWN");
    outw(0xB004, 0x2000);
    outw(0x604, 0x2000);
    outw(0x4004, 0x3400);
}


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
