/**
 * @file kernel/src/drivers/keyboard.c
 * @authors Арен Елчинян (a2.dev@yandex.com)
 * @brief Прослойка для работы с текстовым графическим редактором
 * @version 0.0.2
 * @date 2022-08-24
 *
 * @copyright @copyright Copyright SynapseOS Team (с) 2022
 *
 */
#include <kernel.h>
/*
  0 - none
  1 - shell
  2 - getchar
  3 - gets
  4 - getcode
*/
int32_t input_type = 1, SHIFT = 0, string_mem_counter = 0;
char keycode, last_char; 
char string_mem[256];
char last_string[256];
int32_t lastkey = 0;        // ID последней нажатой кнопки
int32_t lastkeyinset;   // Последний тик нажатия кнопки
int32_t enabledKeyboard = 0;

// Смена состояния для вывода на экран клавиатуры
void changeStageKeyboard(int32_t s){
    qemu_log("changeStageKeyboard: %d",s);
    enabledKeyboard = s;
}
int32_t keyLastInset(){
    if (lastkeyinset+5 > timer_get_ticks()){
        return lastkey;
    }
    return 0;
}

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

/**
 * @brief Очищает последний ввод клавиатуры
 */
void keyboard_clean(){
    char string_mem[128];

    // ndraey-warn: Example above is more correct
    // |            (Invalid variable clean declaration!)
    // v
    //memset(string_mem, 0, 256);
}

void keyboard_install(void) {
    input_type = 1;
    register_interrupt_handler(33, &keyboard_handler_main);
    IRQ_clear_mask(33 - 32); // - 32 потому что после ремаппинга номера смещаются на 32
    qemu_log("Keyboard installed");
}

void keyboard_handler_main(struct regs *r) {

    unsigned char status;

    // Запись EOI 
    outb(0x20, 0x20);

    status = inb(KEYBOARD_STATUS_PORT);

    // Проверяем статус используя нижний бит
    if (status & 0x01) {
        if (enabledKeyboard == 2){
            return;
        }
        keycode = inb(KEYBOARD_DATA_PORT);
        lastkey = keycode;
        lastkeyinset = timer_get_ticks();
        if (enabledKeyboard == 0){
            return;
        }
        //qemu_log("KEY %d", keycode);

        if (input_type == 0) {
            return;
        }
        if (keycode == 72) {
            if (enabledKeyboard == 1){
                tty_printf("72!");
            }
            // Очистка строковой памяти
            
            while (string_mem_counter != 0) {
                string_mem_counter--;
                string_mem[string_mem_counter] = 0;
                tty_backspace();
            }

            if (enabledKeyboard == 1){
                tty_printf("string_mem_counter = %d\n", string_mem_counter);
            }
            string_mem_counter = strlen(last_string);  
            strcpy(string_mem, last_string); 
            if (enabledKeyboard == 1){
                tty_printf("last_string = [%s]\n", last_string);
                tty_printf(last_string);
            }
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
        if (input_type == 4) {
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
            //qemu_log("getch: %d ", keycode);
            input_type = -2;
        }

        if (keycode == ENTER_KEY_CODE) {
            if (enabledKeyboard == 1){
                tty_printf("\nlast_string[%s]string_mem[%s]", last_string, string_mem);
            }
            strcpy(last_string, string_mem);
            if (enabledKeyboard == 1){
                tty_printf("\nlast_string[%s]string_mem[%s]\n", last_string, string_mem);
            }
            if (input_type == 1) {
                string_mem_counter = 0;
                memset(string_mem, 0, 256);
            }
            return;
        }


        if (string_mem_counter >= 256) {
            tty_printf("\nBuffer string_mem is full!!");
            return;
        }

        string_mem[string_mem_counter] = last_char;
        string_mem_counter++;

        if (keycode < 0) {
            return;
        }
        if (enabledKeyboard == 1){
            tty_putchar(last_char);
        }
        qemu_log("key = %c (index %d)", keyboard_map[(unsigned char) keycode], (unsigned char) keycode);
    }
}

int32_t keyboard_getchar() {
    input_type = 2;

    while (input_type != -2) {
        asm volatile("hlt");
        //keyboard_handler_main();
    }

    input_type = 1;
    tty_putchar(last_char);

    return last_char;
}


int32_t keyboard_getscancode() {
    struct regs *r = NULL;

    input_type = 4;

    keyboard_handler_main(r);

    for (int32_t i = 512; i != 0; i--){
        if (keycode == -100){
            keyboard_handler_main(r);
        } else {
            qemu_log("keyboard_getscancode %d, after^ %d ", keycode, i);
            return keycode;
        }
    }
    
    qemu_log("keyboard_getscancode %d ", keycode);
    return keycode;
}


char *keyboard_gets() {
    input_type = 3;
    //qemu_log("2");

    while (input_type != -3) {
        //qemu_log("3");
        asm volatile("hlt");
        //keyboard_handler_main();
    }
    //tty_printf("\nstring [%s]", string_mem);
    
    return string_mem;
}
