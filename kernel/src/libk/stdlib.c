#include <kernel.h>


/**
 * @brief 
 * 
 * @param nmemb 
 * @param size 
 * @return void* 
 */
void* calloc(size_t nmemb, size_t size) {
	void* ptr = kheap_malloc(nmemb * size);
	if (!ptr) {
		return NULL;
	}
	memset(ptr, 0, nmemb * size);
	return ptr;
}


/**
 * @brief 
 * 
 * @param str 
 */
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


/**
 * @brief 
 * 
 * @param n 
 * @param buffer 
 * @return int32_t 
 */
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