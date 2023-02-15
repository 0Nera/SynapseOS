/**
 * @file int.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции обработки прерываний
 * @version 0.1.0
 * @date 21-10-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


#include <arch.h>
#include <libk.h>
#include <tools.h>


static isr_t interrupt_handlers[256];

extern void irq_handler_a(void);
extern void isr_error_a(void);
extern void isr_noerror_a(void);


/**
 * @brief Изменения функции для вектора прерывания
 * 
 * @param index Номер вектора
 * @param func Указатель на функцию
 * @return true В случае успеха
 * @return false В случае если index вне диапазона от 0 до 1023
 */
bool int_set_handler(uint16_t index, void *func) {
    if (index > INTERRUPT_MAX_INDEX) {
        debug_log("index %d is more than %d", index, INTERRUPT_MAX_INDEX);
        return false;
    }

    interrupt_handlers[index] = func;   

    return true;  
}


/**
 * @brief Обработчик ISR прерываний (ошибок)
 * 
 * @param regs Структура-указатель состояний регистров
 */
void isr_handler(int_registers_t regs) {
    if (interrupt_handlers[regs.int_num] != 0) {
        isr_t handler = interrupt_handlers[regs.int_num];
        handler(regs);
    }
}



/**
 * @brief Обработчик IRQ прерываний
 * 
 * @param regs Структура-указатель состояний регистров
 */
void irq_handler(int_registers_t regs) {
    if (regs.int_num >= 40) {
        ports_outb(0xA0, 0x20);
    }
    
    ports_outb(0x20, 0x20);
    
    if (interrupt_handlers[regs.int_num] != 0) {
        isr_t handler = interrupt_handlers[regs.int_num];
        
        handler(regs);
    }
}


/**
 * @brief Регистрация вектора прерываний
 * 
 * @param id Номер прерывания
 * @param handler Обработчик прерывания
 */
void register_interrupt_handler(uint8_t id, isr_t handler) {
  interrupt_handlers[id] = handler;      
}


/**
 * @brief Ошибка при делении на ноль
 * 
 */
static noreturn void division_by_zero() {
    debug_log("[ERROR]Division by zero");
        
    for(;;) {
        halt();
    }
}


/**
 * @brief Неверный код операции
 * 
 */
static noreturn void invalid_opcode() {
    debug_log("[ERROR]Invalid opcode");
        
    for(;;) {
        halt();
    }
}


/**
 * @brief Двойная ошибка(при прерывании или обработке ошибки)
 * 
 */
static noreturn void double_fault(int_registers_t *regs) {
    debug_log("[ERROR]Double fault, int num: %d", regs->int_num);

    for(;;) {
        halt();
    }
}


/**
 * @brief Недопустимое исключение TSS
 * 
 */
static noreturn void invalid_tss() {
    debug_log("[ERROR]Invalid tss");
        
    for(;;) {
        halt();
    }
}


/**
 * @brief Сегмент недоступен
 * 
 */
static noreturn void segment_not_available() {
    debug_log("[ERROR]Segment not available");
        
    for(;;) {
        halt();
    }
}


/**
 * @brief Ошибка стека
 * 
 */
static noreturn void stack_error() {
    debug_log("[ERROR]Stack error");
        
    for(;;) {
        halt();
    }
}


/**
 * @brief Общая ошибка защиты
 * 
 */
static noreturn void general_protection_error(int_registers_t *regs) {
    debug_log("[ERROR]GPT error");

    for(;;) {
        halt();
    }
}


/**
 * @brief Ошибка страницы
 * 
 */
static noreturn void page_fault(int_registers_t *regs) {
    debug_log("[ERROR]Page fault");

    for(;;) {
        halt();
    }
}


/**
 * @brief Ошибка страницы
 * 
 */
static void fpu_error() {
    debug_log("[ERROR]FPU ERROR");
}


/**
 * @brief Инициализация векторов прерываний
 * 
 */
void int_init() {
    for (size_t i = 0; i < 7; i++) {
        idt_set_gate(i, (uint32_t)isr_noerror_a, 0x08, 0x8E);    
    }
    idt_set_gate(8, (uint32_t)isr_error_a, 0x08, 0x8E);    

    for (size_t i = 9; i < 31; i++) {
        idt_set_gate(i, (uint32_t)isr_noerror_a, 0x08, 0x8E);    
    }    

    for (size_t i = 32; i < 47; i++) {
        idt_set_gate(i, (uint32_t)irq_handler_a, 0x08, 0x8E); 
    }
    

    // Установка векторов прерываний для ошибок
    int_set_handler(0,  &division_by_zero);
    int_set_handler(6,  &invalid_opcode);
    int_set_handler(8,  &double_fault);
    int_set_handler(10, &invalid_tss);
    int_set_handler(11, &segment_not_available);
    int_set_handler(12, &stack_error);
    int_set_handler(13, &general_protection_error);
    int_set_handler(14, &page_fault);
    int_set_handler(16, &fpu_error);

}
