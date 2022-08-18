#include <string.h>
#include "../include/stdlib.h"
#include "../include/stdint.h"

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


/*int itoa(int n, char *buffer) {
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
}*/

int32_t itoa(int32_t n, char *buffer) {
    char const digits[] = "0123456789";
    char* p = buffer;

    if (n < 0){
        *p++ = '-';
        n *= -1;
    }

    int s = n;

    do {
        ++p;
        s = s / 10;
    } while(s);

    *p = '\0';

    do { 
        *--p = digits[n % 10];
        n = n / 10;
    } while(n);

    return strlen(buffer);
}

void* malloc(int value) {
	uint32_t address = 0;
    asm volatile("int $0x80" : "=a"(address) : "a"(SC_CODE_malloc), "b"(value));

	return (void*)address;
}

void free(void* memory) {
    asm volatile("int $0x80" 
                :
                : "a"(SC_CODE_free),
                  "b"(memory)
    );
    /*
    asm volatile("mov %%eax, %0" : : "a"(SC_CODE_free), "b"(memory));
    asm volatile("int $0x80");
    */
}

void *calloc(size_t number, size_t size) {
	void* allocated = malloc(number*size);
	if(!allocated) return 0;
	memset(allocated, 0, size*number);
	return allocated;
}
