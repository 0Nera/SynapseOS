#include <interrupts/idt.h>


#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_malloc          2
#define SC_CODE_free            3
#define SC_CODE_version         4
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33


void syscall_init();
void syscall_handler(struct regs *r);