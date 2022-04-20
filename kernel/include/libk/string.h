#pragma once

size_t strlen(const char *str);

int atoi(const char*);

int memcmp(const void*, const void*, size_t);
void* memmove(void* , void* , size_t);

void *memset(void *destination, int c, size_t n);
void* memcpy(void*, const void*, size_t);