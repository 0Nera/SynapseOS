#include <string.h>


void strver(char *str) {
    char c;
    int j = strlen(str) - 1;

    for (int i = 0; i < j; i++) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
        j--;
    }
}


int itoa(int n, char *buffer) {
    int length = n < 0;
    
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
