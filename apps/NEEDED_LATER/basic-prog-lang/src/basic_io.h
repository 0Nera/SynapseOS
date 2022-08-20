#ifndef __BASIC_IO_H_
#define __BASIC_IO_H_

#define MEMORY_SIZE (26)
#define PROGRAM_SIZE (32768)

typedef enum {
  var_null = 0,
  var_double,
  var_string
} variant_type;
typedef char STRING[128];
typedef struct {
  variant_type type;
  union {
    double i;
    STRING s;
  };
} VARIANT;
typedef struct {
  int ln;                       // line number
  STRING line;
} CODE;

extern VARIANT memory[MEMORY_SIZE];
extern CODE code[PROGRAM_SIZE];
extern int cp;
extern int code_size;

void load_program(STRING filename);
void exec_assignment(const STRING line);
void exec_input(const STRING);
void exec_print(const STRING);

#endif
