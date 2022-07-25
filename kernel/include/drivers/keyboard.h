#include <sys/idt.h>
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C

extern unsigned char keyboard_map[128];
extern int32_t input_type;
void keyboard_install(void);
void keyboard_handler(regs_t *r);
void keyboard_handler_main(regs_t *r);
char *keyboard_gets();
int32_t keyboard_getchar();
int32_t keyboard_getscancode();
int32_t keyLastInset();

void changeStageKeyboard(int32_t s);
