/*
    Автор: Арен Елчинян. Распространяется по лицензии GNU GPL 3.0
    Имя файла:  test.c
    Описание:   Программа для теста работы сисфункций
*/


#include <stdint.h>

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


int print_str(char *str) {

 uint32_t result = 0;

   asm volatile("int $0x80" 

          : "=a"(result)         // result = eax (после выполнения)

            : "a"(SC_CODE_puts),      // eax = SC_CODE_puts(0)

             "b"(str)           // ebx = str

            );

   return result;

}

int main() {

   return print_str("Hello world!\n");

}