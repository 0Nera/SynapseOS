#include "../include/stdfile.h"

void fread(const char *filename, int32_t offset, int32_t size, void *buf) {
        asm volatile("int $0x80" 
                :
                : "a"(SC_CODE_readfile),
                  "b"(filename),
                  "c"(offset),
                  "d"(size),
                  "S"(buf)
                );
}