#include <string.h>
#include "../include/stdlib.h"

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

void* malloc(int value) {
	void *mem;
    asm volatile("mov %%eax, %0" : "=a"(mem) : "a"(SC_CODE_malloc), "b"(value));
    asm volatile("int $0x80");
	return mem;
}

void free(void* memory) {
    asm volatile("int $0x80" 
                :
                : "a"(SC_CODE_free),
                  "b"(memory)
    );
    asm volatile("mov %%eax, %0" : : "a"(SC_CODE_free), "b"(memory));
    asm volatile("int $0x80");
}

void *calloc(size_t number, size_t size) {
	return malloc(number*size);
}
