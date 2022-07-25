#include <kernel.h>


void strver(char *str) {
    char c;
    int32_t j = strlen(str) - 1;

    for (int32_t i = 0; i < j; i++) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
        j--;
    }
}


int32_t itoa(int32_t n, char *buffer) {
    int32_t length = n < 0;
    
    if (length) {
        n = -n;
        *buffer = '-';
    }
    
    do {
        buffer[length++] = (n % 10) + '0';
        n /= 10;
    } while (n > 0);

    buffer[length] = '\0';
    strver(buffer);

    return length;
}
