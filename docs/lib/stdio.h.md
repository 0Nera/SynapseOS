# Документацию по stdio.h в SynapseOs
Итак,ты наверное пробовал использовать <stdio.h> для SynapseOs? Я думаю да, раз ты читаешь это. В данном гайде я хочу показать, как работать с этим вашим input/output. Если вы ищете определённую функцию, советую Ctrl-F
Ну, поехали!
> - Так, ну и где лежит stdio.h?
> - Сам он - в kenrel/include/libk/ . А stdio.c лежит в apps/libc/
Тем не менее, сейчас я покажу какие функции есть в stdio!
И да кстати, эта документация будет расширяться по мере добавления новый функций и (моих припадков желания работать)
## Часть первая, printf и print, rand и srand
### void printf(char *text, ...)
Самый часто используемый print, который выдаёт нормальный вывод(это тот же print но, без создания вручную va_list и тп и тд)
Что бы понять как с ним работать прочитайте таблицу из информации о методе print и используйте таким образом:

`printf("%формат", "ТипоТип, который нужен для формата")`

Пример использования:
```c
#include <stdio.h>

int main() {
  printf("HELLO WORLD!!!");
  return 0;
}
```
### void print(char *format, va_list args)
Данный print является часто используемым, но только в своей обёртке, в printf
Так как для каждого типа есть свой принт, данная функция выбирает тип с помощью передаваемого ему аргумент format.
А args нужен для передачи данный для принта(int, char, hex, и тому подобное)
#### Вот таблица форматов, которые можно передавать в format:
- %s - для вывода стандартной строки
- %c - для вывода одного char из строки или просто одного char
- %d - наверное сделали на будущие, что бы выводить тип double, но пока что, выводит стандартный int
- %i - для вывода обычного int
- %u - для вывода unsigned int(Обычный int, но с натуральными и не натуральными числами)
- %x - для вывода типа хекса(hex), требует аргумент с типом uint32_t
Пример использования:
Нету
### void putint(const int i)
Простой вывод int
Пример использования:
```c
#include <stdio.h>

int main() {
  int a,b;
  a = 42;
  b = 42;
  putint(a+b);
  return 0;
}
```
### void print_str(char str[])
Основа для всех методов вывода, выводит строку!
Пример использования:
```c
#include <stdio.h>

int main() {
  char strch[11] = "Hello World" 
  print_str(strch);
  return 0;
}
```
### void putchar(unsigned char ch)
Выводит одну char.
Обёртка над printf
Пример использования:
```c
#include <stdio.h>

int main() {
  char ch;
  ch = getchar();
  printf("Ты ввёл char: ");
  putchar(ch);
  return 0;
}
```
### uint64_t rand()
Функция рандома, в отличии от других OS он уже привязан к времени, что делает srand не обязательным
Пример использования:
```c
#include <stdio.h>

int main() {
  uint64_t a;
  a = rand();
  printf("Рандомное число: %i", a);
  return 0;
}
```
### uint64_t srand(uint32_t seed)
srand здесь не обязателен, rand уже привязян к времени
Пример использования:
Нету
### char getchar()
Возвращает вводимый char из stdin
Пример использования:
```c
#include <stdio.h>

int main() {
  char ch;
  ch = getchar();
  printf("Ты ввёл char: ");
  putchar(ch);
  return 0;
}
```
### char *gets()
Возвращает вводимую строку из stdin
Пример использования:
```c
#include <stdio.h>

int main() {
  char charstr[256];
  printf("Введи строку \n");
  charstr = gets();
  printf("Ты ввёл: %s", charstr);
  return 0;
}
```

