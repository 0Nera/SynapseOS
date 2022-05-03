#include <stdio.h>


/*
    Стандартная библиотека SynapseOS

    stdio - модуль ввода-вывода
*/


int getscancode(){
    void* res = 0;
    int result = -1;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_getscancode));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    //printf("[%d], [%c], [%s], [%x];\n", result, result, result, result);
    return result;
}

char getchar(){
    void* res = 0;
    int result = -1;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_getchar));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    //printf("[%d], [%c], [%s], [%x];\n", result, result, result, result);
    return result;
} 

char *gets() {
    void* res = 0;
    char *result = "";

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_gets));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    return result;
} 

int getversion(){
    void* res = 0;
    int result = -1;
    

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_version));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    return result;
}

int print_str(char *str) {
    void* res = 0;
    int result = -1;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_puts), "b"(&str));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

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



void puthex(uint32_t i) {
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