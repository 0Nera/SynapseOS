#pragma once
#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), \
                                               "=b" (b), \
                                               "=c" (c), \
                                               "=d" (d) : \
                                               "a" (in));
int do_intel(bool silent);
int do_amd(bool silent);
char* printregs(int eax, int ebx, int ecx, int edx);
int detect_cpu(bool silent);
char* getNameBrand();
