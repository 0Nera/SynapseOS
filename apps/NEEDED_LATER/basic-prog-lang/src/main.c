#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "basic_io.h"
#include "lexical.h"
#include "syntax.h"
#include "SynapsePorting.h"

typedef enum {
  key_input = 0,
  key_print,
  key_for,
  key_next,
  key_while,
  key_wend,
  key_if,
  key_else,
  key_endif,
  key_goto,
  key_let
} keywords;

void (*key_func[])(const STRING) = {
  exec_input,
  exec_print,
  exec_for,
  exec_next,
  exec_while,
  exec_wend,
  exec_if,
  exec_else,
  exec_endif,
  exec_goto,
  exec_assignment
};

keywords yacc(const STRING line) {
  if (!strnicmp(line, "INPUT ", 6)) {
    return key_input;
  } else if (!strnicmp(line, "PRINT ", 6)) {
    return key_print;
  } else if (!strnicmp(line, "FOR ", 4)) {
    return key_for;
  } else if (!stricmp (line, "NEXT")) {
    return key_next;
  } else if (!strnicmp(line, "WHILE ", 6)) {
    return key_while;
  } else if (!stricmp(line, "WEND")) {
    return key_wend;
  } else if (!strnicmp(line, "IF ", 3)) {
    return key_if;
  } else if (!stricmp (line, "ELSE")) {
    return key_else;
  } else if (!stricmp (line, "END IF")) {
    return key_endif;
  } else if (!strnicmp(line, "GOTO ", 5)) {
    return key_goto;
  } else if (!strnicmp(line, "LET ", 4)) {
    return key_let;
  } else if (strchr(line, '=')) {
    return key_let;
  }

  return -1;
}

int main (int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s basic_script_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  load_program(argv[1]);

  while (cp < code_size) {
    (*key_func[yacc(code[cp].line)])(code[cp].line);
    cp++;
  }

  return EXIT_SUCCESS;
}
