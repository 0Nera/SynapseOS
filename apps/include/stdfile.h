#ifndef STDFILE_H
#define STDFILE_H

#define SC_CODE_readfile        11
#define SC_CODE_writefile       12

#include "../include/stdlib.h"
#include "../include/stdint.h"

void fread(const char *filename, int32_t offset, int32_t size, void *buf);

#endif