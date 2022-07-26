#ifndef __EXPRESSION_H_
#define __EXPRESSION_H_

#include "basic_io.h"

typedef VARIANT OPERAND;
typedef enum {
  /* 算数运算 */
  oper_lparen = 0,              // 左括号
  oper_rparen,                  // 右括号
  oper_plus,                    // 加
  oper_minus,                   // 减
  oper_multiply,                // 乘
  oper_divide,                  // 除
  oper_mod,                     // 模
  oper_power,                   // 幂
  oper_positive,                // 正号
  oper_negative,                // 负号
  oper_factorial,               // 阶乘
  /* 关系运算 */
  oper_lt,                      // 小于
  oper_gt,                      // 大于
  oper_eq,                      // 等于
  oper_ne,                      // 不等于
  oper_le,                      // 不大于
  oper_ge,                      // 不小于
  /* 逻辑运算 */
  oper_and,                     // 且
  oper_or,                      // 或
  oper_not,                     // 非
  /* 赋值 */
  oper_assignment,              // 赋值
  oper_min                      // 栈底
} operator_type;
typedef enum {
  left2right,
  right2left
} associativity;
typedef struct {
  int numbers;                  // 操作数
  int icp;                      // 优先级
  int isp;                      // 优先级
  associativity ass;            // 结合性
  operator_type oper;           // 操作符
} OPERATOR;
typedef enum {
  token_operand = 1,
  token_operator
} token_type;
typedef struct {
  token_type type;
  union {
    OPERAND var;
    OPERATOR ator;
  };
} TOKEN;
typedef struct tlist {
  TOKEN token;
  struct tlist *next;
} TOKEN_LIST, *PTLIST;

VARIANT eval(const char[]);

#endif
