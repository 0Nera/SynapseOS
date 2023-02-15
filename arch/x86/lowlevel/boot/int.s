.extern isr_handler
.extern irq_handler

.global isr_error_a
.global isr_noerror_a
.global irq_handler_a

# Вызов ISR обработчика ошибок
isr_error_a:
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
isr_noerror_a:

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
irq_handler_a:
      
      pusha
            
      push    %ds
      
      mov    $0x10, %ax
      mov    %ax, %ds
      
      call    irq_handler
      
      pop    %ds      
      
      popa
      
      add    $8, %esp
            
      iret
