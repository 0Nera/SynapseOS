#pragma once

#include <stdint.h>


size_t strlen(const char *str);
void *memcpy(void *destination, const void *source, size_t n);
void *memset(void *destination, int c, size_t n);
int strcmp(const char *str1, const char *str2);char *strcpy(char *dest, const char *src);
int memcmp(const void *s1, const void *s2, size_t n);
size_t str_bksp(char *str, char c);
char *strpbrk(const char *s, const char *accept);
size_t strspn(const char *s, const char *accept);
int strncmp(const char *s1, const char *s2, size_t num);
char *strtok(char *s, const char *delim);
char *strcat(char *s, const char *t);
char *strstr(char *str, char *substr);
char *strcpy(char *dest, const char *src);