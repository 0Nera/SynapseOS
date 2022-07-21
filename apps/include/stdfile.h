#ifndef STDFILE_H
#define STDFILE_H

#define SC_CODE_readfile        11
#define SC_CODE_writefile       12

void fread(const char *filename, int32_t offset, int32_t size, void *buf);

#endif