#include "../include/stdint.h"
size_t strlen(const char *str) {
	size_t len = 0;
	while (str[len] != 0) {
		len++;
	}
	return len;
}


void *memcpy(void *destination, const void *source, size_t n){
    char *tmp_dest = (char*)destination;
    const char *tmp_src = (const char*)source;

    while (n){
        *tmp_dest++ = *tmp_src++;
        n--;
    }

    return destination;
}


void *memset(void *destination, int c, size_t n){
    if (destination){
       char *tmp_mem = (char*)destination;
       while (n--) *tmp_mem++ = c;
    }
    
    return destination;
}


int strcmp(const char *str1, const char *str2) {
    int s1;
    int s2;

    do {
        s1 = *str1++;
        s2 = *str2++;
        if (s1 == 0)
            break;
    } while (s1 == s2);

    return (s1 < s2) ? -1 : (s1 > s2);
}

char *strcpy(char *dest, const char *src) {
    do {
      *dest++ = *src++;
    }
    while (*src != 0);

    return dest;
}


int memcmp(const void *s1, const void *s2, size_t n) { 
    unsigned char u1, u2;

    for (; n-- ; s1++, s2++) {
        u1 = *(unsigned char*) s1;
        u2 = *(unsigned char*) s2;
        if ( u1 != u2) {
            return (u1 - u2);
        }
    }
    return 0;
}



size_t str_bksp(char *str, char c) {
    size_t i = strlen(str);
    i--;
    while (i) {
        i--;
        if (str[i] == c) {
            str[i+1] = 0;
            return 1;
        }
    }
    return 0;
}



char *strpbrk(const char *s, const char *accept) {
    while (*s != '\0') {
        const char *a = accept;
        while (*a != '\0') {
            if (*a++ == *s) {
                return (char*) s;
            }
        }
        ++s;
    }
    return NULL;
}

size_t strspn(const char *s, const char *accept) {
    const char *p;
    const char *a;
    size_t count = 0;

    for (p = s; *p != '\0'; ++p) {
        for (a = accept; *a != '\0'; ++a) {
            if (*p == *a) {
                break;
            }
        }
        if (*a == '\0') {
            return count;
        } else {
            ++count;
        }
    }
    return count;
}
int strncmp(const char *s1, const char *s2, size_t num) {
    for (size_t i=0; i<num; i++) {
        if (s1[i] != s2[i]) {
            return 1;
        }
    }
    return 0;
}

char *strtok(char *s, const char *delim) {
    static char *olds = NULL;
    char *token;

    if (s == NULL) {
        s = olds;
    }

    s += strspn(s, delim);
    if (*s == '\0') {
        olds = s;
        return NULL;
    }

    token = s;
    s = strpbrk(token, delim);
    if (s == NULL) {
        olds = token;
    } else {
        *s = '\0';
        olds = s + 1;
    }
    return token;
}

char *strncpy(char *dest, const char *src, size_t n){
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

char *strcat(char *s, const char *t) {
    strcpy(s + strlen(s), t);
    return s;
}

void substr(char *dest, char *source, int from, int length){
    strncpy(dest, source + from, length);
    dest[length] = 0;
}

char *strstr(char *str, char *substr)
{
    int len = strlen(substr);
    char *ref = substr;
    while(*str && *ref)
    {
        if (*str++ == *ref) ref++;
        if(!*ref) return (str - len);
        if (len == (ref - substr)) ref = substr;
    }
    return NULL;
}

char *strchr(const char *_s, int _c){
    while (*_s != (char)_c){
        if (!*_s++) return 0;
    }
    return (char *)_s;
}

void strtolower(char* as)
{
	while(*as != 0)
	{
		if(*as >= 'A' && *as <= 'Z')
			*as += ('a' - 'A');
		as++;
	}
}

void strtoupper(char* as)
{
	while(*as != 0)
	{
		if(*as >= 'a' && *as <= 'z')
			*as -= ('a' - 'A');
		as++;
	}
}




