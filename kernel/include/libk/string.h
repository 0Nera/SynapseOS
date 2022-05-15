#pragma once

size_t strlen(const char *str);

int32_t atoi(const char*);

int32_t memcmp(const void*, const void*, size_t);
void* memmove(void* , void* , size_t);

void *memset(void *destination, int32_t c, size_t n);
void* memcpy(void*, const void*, size_t);