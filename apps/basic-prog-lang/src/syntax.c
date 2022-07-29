#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #include <ctype.h>
#include "../../include/ctype.h"
#include <math.h>

#include "syntax.h"
#include "SynapsePorting.h"

static struct {
	int id;                       // memory index
	int ln;                       // line number
	double target;                // target value
	double step;
} stack_for[MEMORY_SIZE];
static int top_for = -1;

#define EXIT_FAILURE 1

void exec_for(const STRING line) {
	STRING l;
	char *s, *t;
	int top = top_for + 1;

	if (strnicmp(line, "FOR ", 4)) {
		goto errorhandler;
	} else if (top >= MEMORY_SIZE) {
		printf("FOR 循环嵌套过深！\n");
		exit(EXIT_FAILURE);
	}

	strcpy(l, line);

	s = l + 4;
	while (*s && isspace(*s)) s++;
	if (isalpha(*s) && !isalnum(s[1])) {
		stack_for[top].id = toupper(*s) - 'A';
		stack_for[top].ln = cp;
	} else {
		goto errorhandler;
	}

	do {
		s++;
	} while (*s && isspace(*s));
	if (*s == '=') {
		s++;
	} else {
		goto errorhandler;
	}

	t = strstr(s, " TO ");
	if (t != NULL) {
		*t = 0;
		memory[stack_for[top].id] = eval(s);
		s = t + 4;
	} else {
		goto errorhandler;
	}

	t = strstr(s, " STEP ");
	if (t != NULL) {
		*t = 0;
		stack_for[top].target = eval(s).i;
		s = t + 5;
		stack_for[top].step = eval(s).i;
		if (fabs(stack_for[top].step) < 1E-6) {
			goto errorhandler;
		}
	} else {
		stack_for[top].target = eval(s).i;
		stack_for[top].step = 1;
	}

	if ((stack_for[top].step > 0 && 
		 memory[stack_for[top].id].i > stack_for[top].target)||
		 (stack_for[top].step < 0 && 
		 memory[stack_for[top].id].i < stack_for[top].target)) {
		while (cp < code_size && strcmp(code[cp].line, "NEXT")) {
			cp++;
		}
		if (cp >= code_size) {
			goto errorhandler;
		}
	} else {
		top_for++;
	}

	return;

errorhandler:
	printf("Line %d: 语法错误！\n", code[cp].ln);
	exit(EXIT_FAILURE);
}

void exec_next(const STRING line) {
	if (stricmp(line, "NEXT")) {
		printf("Line %d: 语法错误！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}
	if (top_for < 0) {
		printf("Line %d: NEXT 没有相匹配的 FOR！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}

	memory[stack_for[top_for].id].i += stack_for[top_for].step;
	if (stack_for[top_for].step > 0 && 
		 memory[stack_for[top_for].id].i > stack_for[top_for].target) {
		top_for--;
	} else if (stack_for[top_for].step < 0 && 
		 memory[stack_for[top_for].id].i < stack_for[top_for].target) {
		top_for--;
	} else {
		cp = stack_for[top_for].ln;
	}
}

static struct {
	int ln;
	int isrun;
} stack_while[MEMORY_SIZE];
static int top_while = -1;

void exec_while(const STRING line) {
	const char *s;
	int top = top_while + 1;
	VARIANT v;

	if (strnicmp(line, "WHILE ", 6)) {
		goto errorhandler;
	} else if (top >= MEMORY_SIZE) {
		printf("WHILE 循环嵌套过深！\n");
		exit(EXIT_FAILURE);
	}

	s = line + 6;
	stack_while[top].ln = cp;
	v = eval(s);
	if (v.type == var_double) {
		stack_while[top].isrun = fabs(v.i) > 1E-6;
	} else {
		stack_while[top].isrun = v.s[0] != 0;
	}

	if (!stack_while[top].isrun) {
		while (cp < code_size && stricmp(code[cp].line,"WEND")) {
			cp++;
		}
		if (cp >= code_size) {
			printf("WHILE 没有对应的 WEND\n");
			exit(EXIT_FAILURE);
		} else {
			cp--;
		}
	}
	top_while++;
	return;

errorhandler:
	printf("Line %d: 语法错误！\n", code[cp].ln);
	exit(EXIT_FAILURE);
}

void exec_wend(const STRING line) {
	if (top_while < 0) {
		printf("WEND 没有对应的 WHILE\n");
		exit(EXIT_FAILURE);
	} else if (stricmp(line, "WEND")) {
		printf("Line %d: 语法错误！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}

	if (stack_while[top_while].isrun) {
		cp = stack_while[top_while].ln - 1;
	}
	top_while--;
}

static int stack_if[MEMORY_SIZE];
static int top_if = -1;

void exec_if(const STRING line) {
	STRING l;
	char *s;
	int top = top_if + 1;
	VARIANT v;

	if (strnicmp(line, "IF ", 3)) {
		goto errorhandler;
	} else if (top >= MEMORY_SIZE) {
		printf("IF 嵌套过深！\n");
		exit(EXIT_FAILURE);
	}

	strcpy(l, line);
	s = strstr(l, "THEN");
	if (!s || stricmp(s, "THEN")) {
		goto errorhandler;
	} else {
		*s = 0;
	}

	s = l + 3;
	v = eval(s);
	if (v.type == var_double) {
		stack_if[top] = fabs(v.i) > 1E-6;
	} else {
		stack_if[top] = v.s[0] != 0;
	}

	if (!stack_if[top]) {
		while (cp < code_size && stricmp(code[cp].line,"ELSE")
				&& stricmp(code[cp].line,"END IF")) {
			cp++;
		}
		if (cp >= code_size) {
			printf("IF 没有对应的 END IF\n");
			exit(EXIT_FAILURE);
		} else {
			cp--;
		}
	}
	top_if++;
	return;

errorhandler:
	printf("Line %d: 语法错误！\n", code[cp].ln);
	exit(EXIT_FAILURE);
}

void exec_else(const STRING line) {
	if (top_if < 0) {
		printf("ELSE 没有对应的 IF\n");
		exit(EXIT_FAILURE);
	} else if (stricmp(line, "ELSE")) {
		printf("Line %d: 语法错误！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}

	stack_if[top_if] = !stack_if[top_if];
	if (!stack_if[top_if]) {
		while (cp < code_size && stricmp(code[cp].line,"END IF")) {
			cp++;
		}
		if (cp >= code_size) {
			printf("IF 没有对应的 END IF\n");
			exit(EXIT_FAILURE);
		} else {
			cp--;
		}
	}
}

void exec_endif(const STRING line) {
	if (top_if < 0) {
		printf("END IF 没有对应的 IF\n");
		exit(EXIT_FAILURE);
	} else if (stricmp(line, "END IF")) {
		printf("Line %d: 语法错误！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}

	top_if--;
}

void exec_goto(const STRING line) {
	int ln;

	if (strnicmp(line, "GOTO ", 5)) {
		printf("Line %d: 语法错误！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}

	ln = (int)eval(line + 5).i;
	if (ln > code[cp].ln) {
		while (cp < code_size && ln != code[cp].ln) {
			if (!strnicmp(code[cp].line, "IF ", 3)) {
				top_if++;
				stack_if[top_if] = 1;
			} else if (!stricmp(code[cp].line, "ELSE")) {
				stack_if[top_if] = 1;
			} else if (!stricmp(code[cp].line, "END IF")) {
				top_if--;
			} else if (!strnicmp(code[cp].line, "WHILE ", 6)) {
				top_while++;
				stack_while[top_while].isrun = 1;
				stack_while[top_while].ln = cp;
			} else if (!stricmp(code[cp].line, "WEND")) {
				top_while--;
			} else if (!strnicmp(code[cp].line, "FOR ", 4)) {
				int i = 4;
				VARIANT v;
				while (isspace(code[cp].line[i])) i++;
				v = memory[toupper(code[cp].line[i])-'A'];
				exec_for(code[cp].line);
				memory[toupper(code[cp].line[i])-'A'] = v;
			} else if (!stricmp(code[cp].line, "NEXT")) {
				top_for--;
			}
			cp++;
		}
	} else if (ln < code[cp].ln) {
		while (cp > 0 && ln != code[cp].ln) {
			if (!strnicmp(code[cp].line, "IF ", 3)) {
				top_if--;
			} else if (!stricmp(code[cp].line, "ELSE")) {
				stack_if[top_if] = 1;
			} else if (!stricmp(code[cp].line, "END IF")) {
				top_if++;
				stack_if[top_if] = 1;
			} else if (!strnicmp(code[cp].line, "WHILE ", 6)) {
				top_while--;
			} else if (!stricmp(code[cp].line, "WEND")) {
				int p = cp - 1;
				top_while++;
				while (p > 0 && strnicmp(code[p].line, "WHILE ", 6)) {
					p--;
				}
				stack_while[top_while].isrun = 1;
				stack_while[top_while].ln = p;
			} else if (!strnicmp(code[cp].line, "FOR ", 4)) {
				top_for--;
			} else if (!stricmp(code[cp].line, "NEXT")) {
				int p = cp;
				VARIANT v;
				int i = 4;

				while (cp > 0 && strnicmp(code[cp].line, "FOR ", 4)) {
					cp--;
				}
				while (isspace(code[cp].line[i])) i++;
				v = memory[toupper(code[cp].line[i])-'A'];
				exec_for(code[cp].line);
				memory[toupper(code[cp].line[i])-'A'] = v;
				cp = p;
			}
			cp--;
		}
	} else {
		printf("Line %d: 死循环！\n", code[cp].ln);
		exit(EXIT_FAILURE);
	}

	if (ln == code[cp].ln) {
		cp--;
	} else {
		printf("标号 %d 不存在！\n", ln);
		exit(EXIT_FAILURE);
	}
}
