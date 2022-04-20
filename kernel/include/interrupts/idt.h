#pragma once


#define IDT_NUM_ENTRIES 256

#define TICKS_PER_SECOND 20
#define TIMER_IDT_INDEX    32
#define KEYBOARD_IDT_INDEX 33
#define SYSCALL_IDT_INDEX  0x80 //128
#define MOUSE_IDT_INDEX    44 

// Удерживает регистры во время прерывания
typedef struct regs {
    uint32_t gs, fs, es, ds;                         // Последние отправленные сегменты
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Отправлено pusha
    uint32_t idt_index, err_code;                    // Отправлено вручную
    uint32_t eip, cs, eflags, useresp, ss;           // Отправлено автоматически
} regs;

// Тип Interrupt_handler_t — это указатель на функцию, которая получает struct regs *r и ничего не возвращает (void).
// Таким образом, обработчик прерывания должен получать состояние регистров процессора.
typedef void (*interrupt_handler_t)(struct regs *r);

int register_interrupt_handler(uint32_t idt_index, interrupt_handler_t handler);

// Настройка idt
void idt_init();