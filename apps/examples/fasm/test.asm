; Hello World - FASM
format ELF
public main
main:
	mov eax, 0   ; 0 - сисфункция вывода принта
	mov ebx, "#ILOVESYNAPSEOS\n xd) " ; параметры сисфункции, в нашем случае строка для вывода
	int 80h ; Вызов прерывания
	mov eax, 20   ; 0 - сисфункция rand
	int 80h ; Вызов прерывания
	mov ebx, eax ; Перемещает вывод функции rand в первый аргумент сисфункции(для принта)
	mov eax, 0 ; 0- сисфункция принта
	int 80h ; Вызывает прерывание
	ret
