#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_

#include "basic_io.h"
#include "lexical.h"

void exec_for(const STRING line);
void exec_next(const STRING line);
void exec_while(const STRING line);
void exec_wend(const STRING line);
void exec_if(const STRING line);
void exec_else(const STRING line);
void exec_endif(const STRING line);
void exec_goto(const STRING line);

#endif
