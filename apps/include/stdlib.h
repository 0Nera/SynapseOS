#pragma once

#include <string.h>

#define SC_CODE_malloc          4
#define SC_CODE_free            5

void strver(char *str);
int itoa(int n, char *buffer);
void* malloc(int value);
void* calloc(size_t number, size_t size);
void free(void* memory);
