#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "lexical.h"

#define EXIT_FAILURE 1

static const OPERATOR operators[] = {
	/* 算数运算 */
	{2, 17, 1, left2right, oper_lparen},     // 左括号
	{2, 17, 17, left2right, oper_rparen},    // 右括号
	{2, 12, 12, left2right, oper_plus},      // 加
	{2, 12, 12, left2right, oper_minus},     // 减
	{2, 13, 13, left2right, oper_multiply},  // 乘
	{2, 13, 13, left2right, oper_divide},    // 除
	{2, 13, 13, left2right, oper_mod},       // 模
	{2, 14, 14, left2right, oper_power},     // 幂
	{1, 16, 15, right2left, oper_positive},  // 正号
	{1, 16, 15, right2left, oper_negative},  // 负号
	{1, 16, 15, left2right, oper_factorial}, // 阶乘
	/* 关系运算 */
	{2, 10, 10, left2right, oper_lt},        // 小于
	{2, 10, 10, left2right, oper_gt},        // 大于
	{2, 9, 9, left2right, oper_eq},          // 等于
	{2, 9, 9, left2right, oper_ne},          // 不等于
	{2, 10, 10, left2right, oper_le},        // 不大于
	{2, 10, 10, left2right, oper_ge},        // 不小于
	/* 逻辑运算 */
	{2, 5, 5, left2right, oper_and},         // 且
	{2, 4, 4, left2right, oper_or},          // 或
	{1, 15, 15, right2left, oper_not},       // 非
	/* 赋值 */
	// BASIC 表达式中不会出现赋值
	{2, 2, 2, right2left, oper_assignment},  // 赋值
	/* 最小优先级 */
	{2, 0, 0, right2left, oper_min}          // 栈底
};

static const char *h;
static const char *e;
static TOKEN before;

static TOKEN next_token() {
	TOKEN token = {0};
	STRING s;
	int i;

	if (e == NULL) {
		return token;
	}

	// 去掉前导空格
	while (*e && isspace(*e)) {
		e++;
	}

	if (*e == 0) {
		return token;
	}

	if (*e == '"') {
		token.type = token_operand;
		token.var.type = var_string;
		e++;
		for (i = 0; *e && *e != '"'; i++) {
			token.var.s[i] = *e;
			e++;
		}
		e++;
	} else if (isalpha(*e)) {
		token.type = token_operator;
		for (i = 0; isalnum(*e); i++) {
			s[i] = toupper(*e);
			e++;
		}
		s[i] = 0;
		if (!strcmp(s, "AND")) {
			token.ator = operators[oper_and];
		} else if (!strcmp(s, "OR")) {
			token.ator = operators[oper_or];
		} else if (!strcmp(s, "NOT")) {
			token.ator = operators[oper_not];
		} else if (i == 1) {
			token.type = token_operand;
			token.var = memory[s[0]-'A'];
			if (token.var.type == var_null) {
				memset(&token, 0, sizeof(token));
				printf("变量%c未赋值！\n", s[0]);
				exit(EXIT_FAILURE);
			}
		} else {
			goto errorhandler;
		}
	} else if (isdigit(*e) || *e == '.') {
		token.type = token_operand;
		token.var.type = var_double;
		for (i = 0; isdigit(*e) || *e == '.'; i++) {
			s[i] = *e;
			e++;
		}
		s[i] = 0;
		if (sscanf(s, "%lf", &token.var.i) != 1) {
			// Can't Read
			goto errorhandler;
		}
	} else {
		token.type = token_operator;
		switch(*e) {
		case '(':
			token.ator = operators[oper_lparen];
			break;
		case ')':
			token.ator = operators[oper_rparen];
			break;
		case '+':
			if (before.type == token_operand
			     ||(before.type == token_operator
			     && before.ator.oper == oper_rparen)) {
				token.ator = operators[oper_plus];
			} else {
				token.ator = operators[oper_positive];
			}
			break;
		case '-':
			if (before.type == token_operand
			     ||(before.type == token_operator
			     && before.ator.oper == oper_rparen)) {
				token.ator = operators[oper_minus];
			} else {
				token.ator = operators[oper_negative];
			}
			break;
		case '*':
			token.ator = operators[oper_multiply];
			break;
		case '/':
			token.ator = operators[oper_divide];
			break;
		case '%':
			token.ator = operators[oper_mod];
			break;
		case '^':
			token.ator = operators[oper_power];
			break;
		case '!':
			token.ator = operators[oper_factorial];
			break;
		case '=':
			token.ator = operators[oper_eq];
			break;
		case '>':
			if (*(e+1) == '=') {
				token.ator = operators[oper_ge];
				e++;
			} else {
				token.ator = operators[oper_gt];
			}
			break;
		case '<':
			if (*(e+1) == '=') {
				token.ator = operators[oper_le];
				e++;
			} else if (*(e+1) == '>') {
				token.ator = operators[oper_ne];
				e++;
			} else {
				token.ator = operators[oper_lt];
			}
			break;
		default:
			// Error
			goto errorhandler;
		}
		e++;
	}
	before = token;

	return token;

errorhandler:
	memset(&token, 0, sizeof(token));
	printf("表达式 %s 前有语法错误！不可识别的运算符！\n", h);
	exit(EXIT_FAILURE);
}

PTLIST infix2postfix() {
	PTLIST list = NULL, tail, p;
	PTLIST stack = NULL;

	stack = (PTLIST)calloc(1, sizeof(TOKEN_LIST));
	stack->next = NULL;
	stack->token.type = token_operator;
	stack->token.ator = operators[oper_min];

	memset(&before, 0, sizeof(before));
	for (;;) {
		p = (PTLIST)calloc(1, sizeof(TOKEN_LIST));
		// calloc 自动初始化
		p->next = NULL;
		p->token = next_token();
		if (p->token.type == token_operand) {
			if (!list) {
				list = tail = p;
			} else {
				tail->next = p;
				tail = p;
			}
		} else if (p->token.type == token_operator) {
			if (p->token.ator.oper == oper_rparen) {
				free(p);
				while (stack->token.ator.oper != oper_lparen) {
					p = stack;
					stack = stack->next;
					tail->next = p;
					tail = p;
					tail->next = NULL;
				}
				p = stack;
				stack = stack->next;
				free(p);
			} else {
				while (stack->token.ator.isp >= p->token.ator.icp) {
					tail->next = stack;
					stack = stack->next;
					tail = tail->next;
					tail->next = NULL;
				}
				p->next = stack;
				stack = p;
			}
		} else {
			free(p);
			break;
		}
	}
	while (stack) {
		p = stack;
		stack = stack->next;
		if (p->token.ator.oper != oper_min) {
			p->next = NULL;
			tail->next = p;
			tail = p;
		} else {
			free(p);
		}
	}

	return list;
}

VARIANT eval(const char expr[]) {
	PTLIST list, p;
	PTLIST stack = NULL, op1, op2;
	STRING s1, s2;
	STRING error_msg = "Error: ";
	VARIANT v;

	strcat(error_msg, expr);
	strcat(error_msg, ": ");

	h = e = expr;
	list = infix2postfix();
	while (list) {
		p = list;
		list = list->next;

		if (p->token.type == token_operand) {
			p->next = stack;
			stack = p;
			continue;
		}

		if(stack!=NULL) {printf("Assertion failed at line %d\n", __LINE__); exit(0);}
		//assert(stack != NULL);

		// 操作符
		switch(p->token.ator.oper) {
		/* 算数运算 */
		case oper_plus:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				op1->token.var.i += op2->token.var.i;
			} else {
				if (op1->token.var.type == var_double) {
					sprintf(s1, "%g", op1->token.var.i);
				} else {
					strcpy(s1, op1->token.var.s);
				}
				if (op2->token.var.type == var_double) {
					sprintf(s2, "%g", op2->token.var.i);
				} else {
					strcpy(s2, op2->token.var.s);
				}
				op1->token.type = var_string;
				strcat(s1, s2);
				strcpy(op1->token.var.s, s1);
			}
			free(op2);
			break;
		case oper_minus:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				op1->token.var.i -= op2->token.var.i;
				free(op2);
			} else {
				// Error
				strcat(error_msg, "字符串不支持减运算\n");
				goto errorhandler;
			}
			break;
		case oper_multiply:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				op1->token.var.i *= op2->token.var.i;
				free(op2);
			} else {
				// Error
				strcat(error_msg, "字符串不支持乘运算\n");
				goto errorhandler;
			}
			break;
		case oper_divide:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				if (fabs(op2->token.var.i) < 1E-6) {
					// Error
					strcat(error_msg, "除数为 0\n");
					goto errorhandler;
				} else {
					op1->token.var.i /= op2->token.var.i;
					free(op2);
				}
			} else {
				// Error
				strcat(error_msg, "字符串不支持除运算\n");
				goto errorhandler;
			}
			break;
		case oper_mod:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				if (fabs(op2->token.var.i) < 1E-6) {
					// Error
					strcat(error_msg, "除数为 0\n");
					goto errorhandler;
				} else {
					int a = (int)op1->token.var.i;
					int b = (int)op2->token.var.i;
					op1->token.var.i = a % b;
					free(op2);
				}
			} else {
				// Error
				strcat(error_msg, "字符串不支持求模运算\n");
				goto errorhandler;
			}
			break;
		case oper_power:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				op1->token.var.i = pow(op1->token.var.i,
						op2->token.var.i);
				free(op2);
			} else {
				// Error
				strcat(error_msg, "字符串不支持幂运算\n");
				goto errorhandler;
			}
			break;
		case oper_positive:
			// Ignored
			break;
		case oper_negative:
			if (stack->token.var.type == var_double) {
				stack->token.var.i = -stack->token.var.i;
			} else {
				// 字符串取反操作
				STRING s;
				char *p = stack->token.var.s;
				int len = (int)strlen(p);
				int i;
				for (i = 0; i < len; i++) {
					s[i] = *(p + len - i - 1);
				}
				s[i] = 0;
				strcpy(p, s);
			}
			break;
		case oper_factorial:
			if (stack->token.var.type == var_double) {
				int i = (int)stack->token.var.i;
				stack->token.var.i = 1;
				while (i > 1) {
					stack->token.var.i *= i;
					i--;
				}
			} else {
				// Error
				strcat(error_msg, "字符串不支持阶乘运算\n");
				goto errorhandler;
			}
			break;
		/* 关系运算 */
		case oper_lt:
		case oper_gt:
		case oper_eq:
		case oper_ne:
		case oper_le:
		case oper_ge:
			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double &&
			     op2->token.var.type == var_double) {
				switch(p->token.ator.oper) {
				case oper_lt:
					op1->token.var.i = op1->token.var.i < op2->token.var.i;
					break;
				case oper_gt:
					op1->token.var.i = op1->token.var.i > op2->token.var.i;
					break;
				case oper_eq:
					op1->token.var.i = op1->token.var.i == op2->token.var.i;
					break;
				case oper_ne:
					op1->token.var.i = op1->token.var.i != op2->token.var.i;
					break;
				case oper_le:
					op1->token.var.i = op1->token.var.i <= op2->token.var.i;
					break;
				case oper_ge:
					op1->token.var.i = op1->token.var.i >= op2->token.var.i;
					break;
				}
			} else {
				if (op1->token.var.type == var_double) {
					sprintf(s1, "%g", op1->token.var.i);
				} else {
					strcpy(s1, op1->token.var.s);
				}
				if (op2->token.var.type == var_double) {
					sprintf(s2, "%g", op2->token.var.i);
				} else {
					strcpy(s2, op2->token.var.s);
				}
				op1->token.var.type = var_double;
				switch(p->token.ator.oper) {
				case oper_lt:
					op1->token.var.i = strcmp(op1->token.var.s, op2->token.var.s) < 0;
					break;
				case oper_gt:
					op1->token.var.i = strcmp(op1->token.var.s, op2->token.var.s) > 0;
					break;
				case oper_eq:
					op1->token.var.i = strcmp(op1->token.var.s, op2->token.var.s) == 0;
					break;
				case oper_ne:
					op1->token.var.i = strcmp(op1->token.var.s, op2->token.var.s) != 0;
					break;
				case oper_le:
					op1->token.var.i = strcmp(op1->token.var.s, op2->token.var.s) <= 0;
					break;
				case oper_ge:
					op1->token.var.i = strcmp(op1->token.var.s, op2->token.var.s) >= 0;
					break;
				}
			}
			free(op2);
			break;
		/* 逻辑运算 */
		case oper_and:
		case oper_or: {
			int b1 = 0, b2 = 0;

			op2 = stack;
			op1 = stack = stack->next;

			if (op1->token.var.type == var_double) {
				b1 = fabs(op1->token.var.i) > 1E-6;
			} else {
				b1 = op1->token.var.s[0] != 0;
			}
			if (op2->token.var.type == var_double) {
				b2 = fabs(op2->token.var.i) > 1E-6;
			} else {
				b2 = op2->token.var.s[0] != 0;
			}

			switch(p->token.ator.oper) {
			case oper_and:
				op1->token.var.i = b1 && b2;
				break;
			case oper_or:
				op1->token.var.i = b1 || b2;
				break;
			}

			free(op2);
			break;
		}
		case oper_not:
			op1 = stack;
			if (op1->token.var.type == var_double) {
				op1->token.var.i = fabs(op1->token.var.i) < 1E-6;
			} else {
				op1->token.var.i = op1->token.var.s[0] == 0;
			}
			op1->token.var.type = var_double;
			break;
		default:
			// Unbelievable
			strcat(error_msg, "不识别的运算符！\n");
			goto errorhandler;
			break;
		}
		free(p);
	}
    if(stack && !stack->next) {printf("Assertion failed at line %d\n", __LINE__); exit(0);}
	//assert(stack && !stack->next);
	v = stack->token.var;
	free(stack);

	return v;

errorhandler:
	printf(error_msg);
	exit(EXIT_FAILURE);
}
