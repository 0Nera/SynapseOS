#include <interrupts/idt.h>


#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_version         40

void syscall_init();
void syscall_handler(struct regs *r);