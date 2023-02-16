.extern isr_handler
.extern irq_handler

# Макрос для ISR
.macro ISR_NOERRCODE isr_num

.global    isr\isr_num

isr\isr_num:

    cli
    push    $0    
    push    $\isr_num
    jmp    isr_common_stub_noerr
    
.endm
    

# Макрос для ISR ошибок
.macro ISR_ERRCODE isr_num

.global    isr\isr_num

isr\isr_num:

    cli
    push    $\isr_num
    jmp    isr_common_stub_err
    
.endm

# Макрос для IRQ
.macro IRQ irq_num, isr_num

.global irq\irq_num

irq\irq_num:
    
    cli
    push    $0
    push    $\isr_num
    jmp    irq_common_stub  

.endm


# ISR
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7

ISR_ERRCODE 8

ISR_NOERRCODE 9

ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14

ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18

ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31


ISR_NOERRCODE 80
ISR_NOERRCODE 81
ISR_NOERRCODE 82
ISR_NOERRCODE 83
ISR_NOERRCODE 84
ISR_NOERRCODE 85
ISR_NOERRCODE 86
ISR_NOERRCODE 87
ISR_NOERRCODE 88
ISR_NOERRCODE 89
ISR_NOERRCODE 90


# IRQ
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47


# Вызов ISR обработчика ошибок
isr_common_stub_err:

      pusha
         
      push    %ds
      
      mov    $0x10, %ax
      mov    %ax, %ds      
      
      call    isr_handler
      
      pop    %ds
            
      popa
      
      add    $8, %esp
      
      iret
      

# Вызов ISR обработчика
isr_common_stub_noerr:

      pusha
         
      push    %ds
      
      mov    $0x10, %ax
      mov    %ax, %ds      
      
      call    isr_handler
      
      pop    %ds
            
      popa
      
      add    $8, %esp
      
      iret


# Вызов IRQ обработчика
irq_common_stub:
      
      pusha
            
      push    %ds
      
      mov    $0x10, %ax
      mov    %ax, %ds
      
      call    irq_handler
      
      pop    %ds      
      
      popa
      
      add    $8, %esp
            
      iret
