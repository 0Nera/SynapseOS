#include <interrupts/idt.h>
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C

extern unsigned char keyboard_map[128];
extern int input_type;
void keyboard_install(void);
void keyboard_handler(regs *r);
void keyboard_handler_main(void);
char *keyboard_gets();
char keyboard_getchar();
int keyboard_getscancode();