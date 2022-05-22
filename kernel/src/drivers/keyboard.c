#include <kernel.h>
/*
  0 - none
  1 - shell
  2 - getchar
  3 - gets
*/
int32_t input_type = 1, SHIFT = 0, string_mem_counter = 0;
char keycode, last_char; 
char string_mem[256];


unsigned  char keyboard_map[] = {
    0, 
    27, //`
    '1', '2',  '3', '4',  '5', '6',  '7', '8',	'9',  '0', 
    '-', '=',  
    '\b',	/*  Backspace  */
    '\t',			/*  Tab  */
    'q', 'w',  'e', 'r',	/*  19 */
    't', 'y',  'u', 'i',  'o', 'p',  '[', ']',  '\n',	/*  Enter  key  */
    0,			/*  29 - Control */
    'a', 's',  'd', 'f',  'g', 'h',  'j', 'k',  'l', ';',	/*  39 */
    '\'',  '`', 
    -2,		/*  Left shift 42 */
    '\\',  'z', 'x',  'c', 'v',  'b', 'n',			/*  49 */
    'm', ',',  '.', '/',  
    -2,				/*  Right  shift  54*/
    '*',
    0,	/*  Alt  */
    ' ',	/*  Space  bar  */
    -1,	/*  Caps lock  */
    0,	/*  59 - F1  key  ...  >  */
    0, 0,  0, 0,  0, 0,  0, 0,
    0,	/*  <  ...  F10  */
    0,	/*  69 - Num lock*/
    0,	/*  Scroll Lock  */
    0,	/*  Home key */
    0,	/*  Up Arrow */
    0,	/*  Page Up  */
    '-',
    0,	/*  Left Arrow */
    0,
    0,	/*  Right  Arrow  */
    '+',
    0,	/*  79 - End key*/
    0,	/*  Down Arrow */
    0,	/*  Page Down  */
    0,	/*  Insert Key */
    0,	/*  Delete Key */
    0, 0,  0,
    0,	/*  F11  Key  */
    0,	/*  F12  Key  */
    0,	/*  All  other  keys are undefined */
};

unsigned  char keyboard_map_shifted[] = {
    0, 
    27, //`
    '!', '@',  '#', '$',  '%', '^',  '&', '*',	'(',  ')', 
    '_', '+',  
    '\b',	/*  Backspace  */
    '\t',			/*  Tab  */
    'Q', 'W',  'E', 'R',	/*  19 */
    'T', 'Y',  'U', 'I',  'O', 'P',  '{', '}',  '\n',	/*  Enter  key  */
    0,			/*  29 - Control */
    'A', 'S',  'D', 'F',  'G', 'H',  'J', 'K',  'L', ':',	/*  39 */
    '"',  '`', 
    1,		/*  Left shift */
    '|',  'Z', 'X',  'C', 'V',  'B', 'N',			/*  49 */
    'M', '<',  '>', '?',  
    1,				/*  Right  shift  */
    '*',
    0,	/*  Alt  */
    ' ',	/*  Space  bar  */
    -1,	/*  Caps lock  */
    0,	/*  59 - F1  key  ...  >  */
    0, 0,  0, 0,  0, 0,  0, 0,
    0,	/*  <  ...  F10  */
    0,	/*  69 - Num lock*/
    0,	/*  Scroll Lock  */
    0,	/*  Home key */
    0,	/*  Up Arrow */
    0,	/*  Page Up  */
    '-',
    0,	/*  Left Arrow */
    0,
    0,	/*  Right  Arrow  */
    '+',
    0,	/*  79 - End key*/
    0,	/*  Down Arrow */
    0,	/*  Page Down  */
    0,	/*  Insert Key */
    0,	/*  Delete Key */
    0, 0,  0,
    0,	/*  F11  Key  */
    0,	/*  F12  Key  */
    0,	/*  All  other  keys are undefined */
};



void keyboard_install(void) {
    input_type = 1;
    // 0xFD это 11111101 - включает IRQ1 (клавиатура)
    outb(0x21, 0xFD);
}

void keyboard_handler_main(void) {

    unsigned char status;

    // Запись EOI 
    outb(0x20, 0x20);

    status = inb(KEYBOARD_STATUS_PORT);

    // Проверяем статус используя нижний бит
    if (status & 0x01) {
        keycode = inb(KEYBOARD_DATA_PORT);
        //qemu_printf("KEY %d\n", keycode);
        if (input_type == 0) {
            return;
        }
        if (input_type == 2) {
            if (keycode < 0){
                return;
            }
            if (keycode == 42) {
                SHIFT = 1;
                return;
            } else {
                if (SHIFT) {
                    last_char =  keyboard_map_shifted[(unsigned char) keycode];
                } else {
                    last_char = keyboard_map[(unsigned char) keycode];
                }
                SHIFT = 0;
            }
            input_type = -2;
            
            return;
        }
        if (keycode == 28) {
            input_type = -3;
            return;
        }
        if (keycode == -100){
            memset(string_mem, 0, 256);
            string_mem_counter = 0;
            return;
        }

        if (keycode == 42) {
            SHIFT = 1;
            return;
        }
        if (keycode < 0){
            return;
        }
        if (SHIFT) {
            last_char =  keyboard_map_shifted[(unsigned char) keycode];
        } else {
            last_char = keyboard_map[(unsigned char) keycode];
        }
        SHIFT = 0;
        
        if (input_type == 2) {
            input_type = 0;
            
            return;
        }
        // Кейкод 14 это бекспейс
        if (keycode == 14) {

            if (string_mem_counter != 0) {
                string_mem_counter--;
                string_mem[string_mem_counter] = 0;

                tty_backspace();
            }
            return;
        }
        if (input_type == 3) {
            qemu_printf("getch: %d ", keycode);
            input_type = -2;
            return;
        }

        if (keycode == ENTER_KEY_CODE) {

            if (input_type == 1) {
                string_mem_counter = 0;
                memset(string_mem, 0, 256);
            }
            return;
        }

        if (keycode == 71) {
            qemu_printf("\n\nEXIT");
            reboot();
            return;
        }

        if (string_mem_counter >= 256) {
            tty_printf("Buffer string_mem is full!!");
            return;
        }

        string_mem[string_mem_counter] = last_char;
        string_mem_counter++;

        if (keycode < 0) {
            return;
        }
        tty_putchar(last_char);

        qemu_printf("key = %c (index %d)\n", keyboard_map[(unsigned char) keycode], (unsigned char) keycode);
    }
}

int32_t keyboard_getchar() {
    input_type = 2;

    while (input_type != -2) {
        keyboard_handler_main();
    }

    input_type = 1;
    tty_putchar(last_char);

    return last_char;
}

int32_t keyboard_getscancode() {
    input_type = 2;

    keyboard_handler_main();

    input_type = 1;
    
    return keycode;
}

char *keyboard_gets() {

    input_type = 3;

    while (input_type != -3) {
        keyboard_handler_main();
    }
    //tty_printf("\nstring [%s]", string_mem);
    
    return string_mem;
}