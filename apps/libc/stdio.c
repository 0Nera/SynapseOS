#include <stdio.h>


/*
    Стандартная библиотека SynapseOS

    stdio - модуль ввода-вывода
*/

/*
    Регистры сисфункций
    eax - номер
    ebx - параметр 1
    edx - параметр 2
    ecx - параметр 3
    esi - параметр 4
    edi - параметр 5
    ebp - параметр 6

    Символьный код | Регистры
    a                eax, ax, al
    b                ebx, bx, bl
    c                ecx, cx, cl
    d                edx, dx, dl
    S                esi, si
    D                edi, di
*/

#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_setdev          10
#define SC_CODE_readfile        11
#define SC_CODE_writefile       12
#define SC_CODE_rand            20
#define SC_CODE_srand           21
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33 
#define SC_CODE_version         40



int getscancode(){
    int result = 0;

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_getscancode)
                );

    return result;
}


char getchar(){
    char result = '\0';

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_getchar)
                );

    return result;
} 

char *gets() {
    char *result = "";

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_gets)
                );

    return result;
} 


int getversion(){
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_version)
                );

    return result;
}


int print_str(char *str) {
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)                  // result = eax (после выполнения)
                : "a"(SC_CODE_puts),            // eax = SC_CODE_puts(0)
                  "b"(str)                      // ebx = str
                );

    return result;
}


int rand(){
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_rand)
                );

    return result;
}


int srand(uint32_t seed) {
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)                   // result = eax (после выполнения)
                : "a"(SC_CODE_srand),            // eax = SC_CODE_puts(0)
                  "b"(seed)                      // ebx = str
                );

    return result;
}


/*
    putint - вывод числа
*/
void putint(const int i) {
    char res[32];

    if (i < 0) {
        print_str('-');
    }

    itoa(i, res);
    print_str(res);
    
}



void puthex(unsigned int i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned int n, d = 0x10000000;

    print_str("0x");

    while((i / d == 0) && (d >= 0x10)) d /= 0x10;

    n = i;

    while( d >= 0xF ) {
        print_str(hex[n/d]);
        n = n % d;
        d /= 0x10;
    }

    print_str(hex[n]);
}


void print(char *format, va_list args) {
    int i = 0;
    char temp[2] = {0};

    while (format[i]) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's':
                    print_str(va_arg(args, char*));
                    break;
                case 'c':
                    temp[0] = va_arg(args, char*);
                    print_str(temp);
                    break;
                case 'd':
                    putint(va_arg(args, int));
                    break;
                case 'i':
                    putint(va_arg(args, int));
                    break;
                case 'u':
                    putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    puthex(va_arg(args, uint32_t));
                    break;
                default:
                    temp[0] = format[i];
                    print_str(temp);
            }
        } else {
            temp[0] = format[i];
            print_str(temp);
        }
        i++;
    }
}


/*
    printf - форматированный вывод
*/
void printf(char *text, ...) {
    va_list args;
    va_start(args, text);
    print(text, args);
}