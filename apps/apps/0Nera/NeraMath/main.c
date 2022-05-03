#include <stdio.h>
#include <string.h>
#include "messages.h"


void process_op(char op, int *x, int *y) {
	if (op == '+') {
		*x += *y; *y = 0;
	} else if (op == '-') {
		*x -= *y; *y = 0;
	} else if (op == '*') {
		*x *= *y; *y = 0;
	} else if (op == '/') {
		if (*x != 0 && *y != 0) {
			*x /= *y; *y = 0;
		} else {
			printf("%s\n",ZERO_DIVISION);
		}
	}
}

int eval_expr(char *inp){
	
	int size, i, x = 0, y = 0;
	
	char op = '+';
	//Узнаем размер массива
	size = strlen(inp); // strlen посчитает столько там ЕСТЬ символов сейчас
	//Парсинг строк
	for (i = 0; i < size; i++){
		char c = inp[i];
		// если встретили знак операции
		if (c == '+' || c == '-' || c == '*' || c == '/'){
			// обрабатываем прошлую операцию
			process_op(op, &x, &y);
			op = c; // запоминаем новую операцию
		} else if (c == '\n' || c == '\t' || c == '\a' || c == ' '){
			printf("%s '%c'\n", INVALID_CHARACTER, inp[i]);
		} else {
			y= y*10 + (c - '0');
			printf("y = %d\n", y);
		}
	}
	process_op(op, &x, &y);
	
	return x;
}



int main(){
	//printf("Argc = %d, argv[1] = %s\n", argc, argv[1]);
	//Входные данные
	char input_text[30000];
	
	//Сигнал выхода
	int exit_signal = 0;
	
	printf("%s", info);
	printf("%s", help);
	
	while (exit_signal == 0){
		printf("\n > ");
		strcpy(input_text, gets());
			
		//Выход из программы
		if (input_text[0] == 'e'){
			exit_signal =  1;
		} else if (input_text[0] == '?'){ 
			//Справка
			printf("%s", info);
			printf("%s", help);
		} else {
			printf("\nResult: %d\n", eval_expr(input_text));
		}
	}
	
	
	return 0;
}
