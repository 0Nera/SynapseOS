; Hello World - FASM

format ELF

public main


main:
    mov eax, 0      ; 0 - сисфункция
    mov ebx, hello  ; параметры сисфункции
    int 80h
    ret


hello db 'Hi',0