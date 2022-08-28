#include <kernel.h>

size_t strlen(const char *str){
    size_t len = 0;
    while (str[len] != 0){
        len++;
    }
    return len;
}

void *memcpy(void *destination, const void *source, size_t n){
    char *tmp_dest = (char *)destination;
    const char *tmp_src = (const char *)source;

    while (n--) {
        *tmp_dest++ = *tmp_src++;
    }

    return destination;
}

void *memset(void *destination, int32_t c, size_t n){
    if (destination){
        char *tmp_mem = (char *)destination;
        while (n--)
            *tmp_mem++ = c;
    }

    return destination;
}

int32_t strcmp(const char *str1, const char *str2){
    int32_t s1;
    int32_t s2;

    do{
        s1 = *str1++;
        s2 = *str2++;
        if (s1 == 0)
            break;
    } while (s1 == s2);

    return (s1 < s2) ? -1 : (s1 > s2);
}

bool strcmpn(const char *str1, const char *str2){
    int32_t s1;
    int32_t s2;

    do{
        s1 = *str1++;
        s2 = *str2++;
        if (s1 == 0)
            break;
    } while (s1 == s2);

    return (s1 < s2) ? false : (s1 > s2);
}



char *strcpy(char *dest, const char *src){
    do{
        *dest++ = *src++;
    } while (*src != 0);

    return dest;
}

int32_t memcmp(const void *s1, const void *s2, size_t n){
    unsigned char u1, u2;

    for (; n--; s1++, s2++){
        u1 = *(unsigned char *)s1;
        u2 = *(unsigned char *)s2;
        if (u1 != u2){
            return (u1 - u2);
        }
    }
    return 0;
}

size_t str_bksp(char *str, char c){
    size_t i = strlen(str);
    i--;
    while (i){
        i--;
        if (str[i] == c){
            str[i + 1] = 0;
            return 1;
        }
    }
    return 0;
}

char *strpbrk(const char *s, const char *accept){
    while (*s != '\0'){
        const char *a = accept;
        while (*a != '\0'){
            if (*a++ == *s){
                return (char *)s;
            }
        }
        ++s;
    }
    return NULL;
}

size_t strspn(const char *s, const char *accept){
    const char *p;
    const char *a;
    size_t count = 0;

    for (p = s; *p != '\0'; ++p){
        for (a = accept; *a != '\0'; ++a){
            if (*p == *a){
                break;
            }
        }
        if (*a == '\0'){
            return count;
        }
        else{
            ++count;
        }
    }
    return count;
}

int32_t strncmp(const char *s1, const char *s2, size_t num){
    for (size_t i = 0; i < num; i++){
        if (s1[i] != s2[i]){
            return 1;
        }
    }
    return 0;
}

char *strtok(char *s, const char *delim){
    static char *olds = NULL;
    char *token;

    if (s == NULL){
        s = olds;
    }

    s += strspn(s, delim);
    //qemu_log("NOW S IS: %s", s);

    if (*s == '\0'){
        olds = s;
        return NULL;
    }

    token = s;
    s = strpbrk(token, delim);
    if (s == NULL){
        olds = token;
    }
    else{
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

char *strcat(char *s, const char *t){
    strcpy(s + strlen(s), t);
    return s;
}

void substr(char *dest, char *source, int from, int length){
    strncpy(dest, source + from, length);
    dest[length] = 0;
}

char *strchr(const char *_s, int _c){
    while (*_s != (char)_c){
        if (!*_s++){
            return 0;
        }
    }
    return (char *)_s;
}


uint32_t format_string_size(char *text, va_list args){
    const unsigned char hex[18] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    uint32_t i = 0;
    uint32_t size = 0;
    uint32_t n, d = 0x10000000;
    while (text[i]){
        char res[32];
        int temp_int = 0;
        char temp_char = '0';
        if (text[i] == '%'){
            i++;
            switch (text[i]){
            case 's':
                size += strlen(va_arg(args, char *));
                break;
            case 'c':
                temp_char = va_arg(args, char);
                size++;
                break;
            case 'd':
                temp_int = va_arg(args, int);

                itoa(temp_int, res);
                size += strlen(res);
                break;
            case 'i':
                temp_int = va_arg(args, int);

                itoa(temp_int, res);
                size += strlen(res);
                break;
            case 'u':
                temp_int = va_arg(args, unsigned int);

                itoa(temp_int, res);
                size += strlen(res);
                break;
            case 'x':

                temp_int = va_arg(args, unsigned int);
                n = 0;
                d = 0x10000000;

                res[0] = '0';
                res[1] = 'x';

                while ((temp_int / d == 0) && (d >= 0x10))
                    d /= 0x10;

                n = temp_int;

                while (d >= 0xF)
                {
                    //strcpy(res, hex[n / d]); // hex[n / d] is unsigned char, but strcpy parameter wants const char*
                    strcpy(res, (char*)&hex[n / d]);
                    n = n % d;
                    d /= 0x10;
                }

                size += strlen(res);
                break;
            case 'v':
                temp_int = va_arg(args, unsigned int);
                n = 0;
                d = 0x10000000;

                while ((temp_int / d == 0) && (d >= 0x10))
                    d /= 0x10;

                n = temp_int;

                while (d >= 0xF)
                {
                    //strcpy(res, hex[n / d]);
                    strcpy(res, (char*)&hex[n / d]);
                    n = n % d;
                    d /= 0x10;
                }

                size += strlen(res);
                break;
            default:
                size++;
            }
            // \n
        }
        else if (text[i] == 10){
            size++;
        }
        else if (text[i] == 9){
            size += 4;
        }
        else{
            size++;
        }
        i++;
    }
    return size;
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


char *format_string(char *text, ...){
    va_list args;
    uint32_t i = 0;
    va_start(args, text);
    uint32_t size = format_string_size(text, args);
    qemu_log("format:%d size", size);
    char *result = kheap_malloc(size);
    memset(result, 0, strlen(text));

    while (text[i]){
        char res[32];
        int temp_int = 0;

        if (text[i] == '%'){
            i++;
            switch (text[i]){
                case 's':
                    strcat(result, va_arg(args, char*));
                    break;
                case 'c': {
                    //strcat(result, va_arg(args, char)) // Wrong!
                    result[i] = va_arg(args, char);  // Maybe correct                  
                    break;
                }
                case 'd':{
                    temp_int = va_arg(args, int);
                    itoa(temp_int, res);
                    strcat(result, res);
                    break;
                }
                case 'i':
                    //tty_putint(va_arg(args, int));
                    break;
                case 'u':
                    //tty_putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    result[i]='!';
                    //tty_puthex(va_arg(args, uint32_t));
                    break;
                case 'v':
                    //tty_puthex_v(va_arg(args, uint32_t));
                    break;
                default:
                    result[i] = text[i];
            }
        }
        else if (text[i] == 10){
            // tty_line_fill[tty_pos_y] = tty_pos_x;
            // tty_pos_x = 0;

            // if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) {
            // tty_scroll();
            //} else {
            // tty_pos_y += 17;
            //}
        }
        else if (text[i] == 9){
            // tty_pos_x += 4 * 17;
        }
        else{
            result[i] = text[i];
        }
        i++;
    }
    va_end(args);
    return result;
}
