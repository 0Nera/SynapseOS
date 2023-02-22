#include <libk.h>
#include <print.h>

static putc_ptr putc;

static void puts(const char string[]) {
    while (*string)
        putc(*string++);
}

static void printunum(uint32_t i, int32_t r) {
    const unsigned char symbols[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    char str[33] = "";
    int index = sizeof(str) - 1;

    do {
        str[--index] = symbols[i % r];
        i /= r;
    } while (i);

    puts(str + index);
}

static void printnum(int32_t i, int32_t r) {
    if (i < 0)
        putc('-');
    printunum(-i, r);
}

static void printfloat(double num, int after_point) {
    int int_float = (int)num;
    printnum(int_float, 10);
    putc('.');
    printunum(num * pow(10, after_point), 10);
}

void printf(putc_ptr f, const char *format_string, va_list args) {
    putc = f;
    while (*format_string) {
        if (*format_string == '%') {
            switch (*++format_string) {
            case 'x':
                printunum(va_arg(args, uint32_t), 16);
                break;
            case '8':
                printnum((unsigned char)va_arg(args, char*)[0], 10);
                break;
            case 'd':
                printnum(va_arg(args, int), 10);
                break;
            case 'l':
                printnum(va_arg(args, long), 10);
                break;
            case 'f':
                printfloat(va_arg(args, double), 7);
                break;
            case 'u':
                printunum(va_arg(args, unsigned int), 10);
                break;
            case 'z':
                printunum(va_arg(args, size_t), 10);
                break;
            case 's':
                puts(va_arg(args, char*));
                break;
            case 'c':
                putc((char)va_arg(args, int));
                break;
            }
        } else {
            putc(*format_string);
        }
        format_string++;
    }
}
