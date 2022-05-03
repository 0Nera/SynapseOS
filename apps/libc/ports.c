#include <ports.h>


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


int com1_is_transmit_empty() {
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


void sleep(int i) {
    for (int j = i * 1000; j != 0; j--){
        io_wait();
    }
}


//Play sound using built in speaker 
void play_sound(uint32_t nFrequence) {	
    uint32_t Div;	
    uint8_t tmp;   
    //Set the PIT to the desired frequency	
    Div = 1193180 / nFrequence;	
    outb(0x43, 0xb6);	
    outb(0x42, (uint8_t) (Div) );	
    outb(0x42, (uint8_t) (Div >> 8));   
    //And play the sound using the PC speaker	
    tmp = inb(0x61); 	
    if (tmp != (tmp | 3)) {		
        outb(0x61, tmp | 3);	
    }
}


//make it shutup
void nosound() {	
    uint8_t tmp = inb(0x61) & 0xFC;	
    outb(0x61, tmp);
}


//Make a beep
void beep() {
    play_sound(1000);
    sleep(5000);
    nosound();
}