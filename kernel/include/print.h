#ifndef PRINT_H
#define PRINT_H

typedef (*putc_ptr)(char);
void printf(putc_ptr, char*, ...);

#endif
