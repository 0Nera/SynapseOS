#pragma once


#define IDT_NUM_ENTRIES 256

#define TICKS_PER_SECOND 20
#define TIMER_IDT_INDEX    32
#define KEYBOARD_IDT_INDEX 33
#define SYSCALL_IDT_INDEX  0x80 //128
#define MOUSE_IDT_INDEX    44 

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1

#define PIC_CMD_EOI 0x20

#define SET_IDT_ENTRY(idx) \
    set_idt_entry(idx, (uint32_t) &interrupt_handler_##idx,\
                  0x08, 0x8E);

#define DECLARE_INTERRUPT_HANDLER(i) void interrupt_handler_##i(void)

#define ICW1 0x11
#define ICW4 0x01
#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave  PIC */

#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)


// Удерживает регистры во время прерывания
typedef struct regs {
    uint32_t gs, fs, es, ds;                         // Последние отправленные сегменты
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Отправлено pusha
    uint32_t idt_index, err_code;                    // Отправлено вручную
    uint32_t eip, cs, eflags, useresp, ss;           // Отправлено автоматически
} regs;


struct idt_entry {
    uint16_t handler_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t handler_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


typedef struct idt_ptr idt_ptr_t;
typedef struct idt_entry idt_entry_t;

// Тип Interrupt_handler_t — это указатель на функцию, которая получает struct regs *r и ничего не возвращает (void).
// Таким образом, обработчик прерывания должен получать состояние регистров процессора.
typedef void (*interrupt_handler_t)(struct regs *r);

int32_t register_interrupt_handler(uint32_t idt_index, interrupt_handler_t handler);

// Настройка idt
void idt_init();