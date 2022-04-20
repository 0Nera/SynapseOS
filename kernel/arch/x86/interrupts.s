# Работа с прерываниями 


# Объявление функций
.global gdt_flush 
.global tss_flush
.global idt_load

gdt_flush:
    cli

    mov 4(%esp), %eax
    lgdt (%eax)

    mov $0x10, %ax # 0x10 это смещение в GDT к нашему сегменту данных
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    jmp $0x08, $.flush

.flush:
    ret 


# 0x28 = 00101000 . биты 0 и 1 (два крайних слева) равны нулю, поэтому селектор равен 0.
# 00101000 = 40 в десятичном виде - это смещение в байтах от начала GDT.
# каждая запись gtd является 8-битной. 40/8 = 5. Это означает, что селектор 0x28 равен ring0 и относится к gdt[5].


tss_flush:
    mov $0x2b, %ax 
    ltr %ax
    ret




idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret



.macro no_error_code_handler idt_index
.global interrupt_handler_\idt_index
interrupt_handler_\idt_index:
    cli
    push $0
    push $\idt_index
    jmp  default_interrupt_handler
.endm

.macro error_code_handler idt_index
.global interrupt_handler_\idt_index
interrupt_handler_\idt_index:
    cli
    push $\idt_index
    jmp  default_interrupt_handler
.endm

.extern irq_handler

default_interrupt_handler:
    pushal
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov %esp, %eax 
    push %eax

    call run_interrupt_handler

    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds

    popal
    add $8, %esp 

    sti

    iret 

# ISRs
no_error_code_handler 0
no_error_code_handler 1
no_error_code_handler 2
no_error_code_handler 3
no_error_code_handler 4
no_error_code_handler 5
no_error_code_handler 6
no_error_code_handler 7
error_code_handler 8
no_error_code_handler 9
error_code_handler 10
error_code_handler 11
error_code_handler 12
error_code_handler 13
error_code_handler 14
no_error_code_handler 15
no_error_code_handler 16
error_code_handler 17
no_error_code_handler 18
no_error_code_handler 19

no_error_code_handler 20
no_error_code_handler 21
no_error_code_handler 22
no_error_code_handler 23
no_error_code_handler 24
no_error_code_handler 25
no_error_code_handler 26
no_error_code_handler 27
no_error_code_handler 28
no_error_code_handler 29
no_error_code_handler 30
no_error_code_handler 31


no_error_code_handler 32
no_error_code_handler 33
no_error_code_handler 34
no_error_code_handler 35
no_error_code_handler 36
no_error_code_handler 37
no_error_code_handler 38
no_error_code_handler 39
no_error_code_handler 40
no_error_code_handler 41
no_error_code_handler 42
no_error_code_handler 43
no_error_code_handler 44
no_error_code_handler 45
no_error_code_handler 46
no_error_code_handler 47

#no_error_code_handler 48

#no_error_code_handler 127
no_error_code_handler 128 
