Hello World на сисфункциях SynapseOS

Здравствуйте друзья! Сегодня мы напишем hello world используя clang и сисфункцию вывода для SynapseOS.

Но перед этим, прочтите статью по установке SynapseOS, а также сборке ядра

Перед написанием любой программы нужно установить средства сборки.

На Ubuntu это:

sudo apt install llvm lld  

Далее перейдем к теории.

Сисфункции в SynapseOS вызываются через прерывание 0x80.

Регистры сисфункций:

  eax - номер сисфункций

  ebx - параметр 1

  edx - параметр 2

  ecx - параметр 3

  esi - параметр 4

  edi - параметр 5

  ebp - параметр 6

В eax также идет результат выполнения.

Пример вызова сисфункции:

mov eax, 42 ; Получаем количество тиков

int 80h ; Вызов прерывания

Нас интересует сисфункция под номером 0 - вывод строки в консоль.

На языке С это выглядит так:

int print_str(char *str) {

  uint32_t result = 0;

  asm volatile("int $0x80" 

        : "=a"(result)         // result = eax (после выполнения)

        : "a"(SC_CODE_puts),      // eax = SC_CODE_puts(0)

         "b"(str)           // ebx = str

        );

  return result;

}

int main() {

  return print_str("Hello world!\n");

}

Результат:

На ассемблере FASM:

; Hello World - FASM

format ELF

public main

main:

  mov eax, 0   ; 0 - сисфункция

  mov ebx, hello ; параметры сисфункции

  int 80h

  ret

hello db 'Hi',0

Результат:

