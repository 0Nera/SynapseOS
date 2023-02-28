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

extern void isr0(void); ///< ISR
extern void isr1(void); ///< ISR
extern void isr2(void); ///< ISR
extern void isr3(void); ///< ISR
extern void isr4(void); ///< ISR
extern void isr5(void); ///< ISR
extern void isr6(void); ///< ISR
extern void isr7(void); ///< ISR

extern void isr8(void); ///< ISR
extern void isr9(void); ///< ISR
extern void isr10(void); ///< ISR
extern void isr11(void); ///< ISR
extern void isr12(void); ///< ISR
extern void isr13(void); ///< ISR
extern void isr14(void); ///< ISR
extern void isr15(void); ///< ISR

extern void isr16(void); ///< ISR
extern void isr17(void); ///< ISR
extern void isr18(void); ///< ISR
extern void isr19(void); ///< ISR
extern void isr20(void); ///< ISR
extern void isr21(void); ///< ISR
extern void isr22(void); ///< ISR
extern void isr23(void); ///< ISR

extern void isr24(void); ///< ISR
extern void isr25(void); ///< ISR
extern void isr26(void); ///< ISR
extern void isr27(void); ///< ISR
extern void isr28(void); ///< ISR
extern void isr29(void); ///< ISR
extern void isr30(void); ///< ISR
extern void isr31(void); ///< ISR

extern void isr80(void); ///< Зарезирвировано
extern void isr81(void); ///< Зарезирвировано
extern void isr82(void); ///< Зарезирвировано
extern void isr83(void); ///< Зарезирвировано
extern void isr84(void); ///< Зарезирвировано
extern void isr85(void); ///< Зарезирвировано
extern void isr86(void); ///< Зарезирвировано
extern void isr87(void); ///< Зарезирвировано
extern void isr89(void); ///< Зарезирвировано
extern void isr90(void); ///< Зарезирвировано

extern void irq0(void); ///< IRQ
extern void irq1(void); ///< IRQ
extern void irq2(void); ///< IRQ
extern void irq3(void); ///< IRQ
extern void irq4(void); ///< IRQ
extern void irq5(void); ///< IRQ
extern void irq6(void); ///< IRQ
extern void irq7(void); ///< IRQ
extern void irq8(void); ///< IRQ
extern void irq9(void); ///< IRQ
extern void irq10(void); ///< IRQ
extern void irq11(void); ///< IRQ
extern void irq12(void); ///< IRQ
extern void irq13(void); ///< IRQ
extern void irq14(void); ///< IRQ
extern void irq15(void); ///< IRQ

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

    for (;;) {
        halt();
    }
}

/**
 * @brief Неверный код операции
 *
 */
static noreturn void invalid_opcode() {
    debug_log("[ERROR]Invalid opcode");

    for (;;) {
        halt();
    }
}

/**
 * @brief Двойная ошибка(при прерывании или обработке ошибки)
 *
 */
static noreturn void double_fault(int_registers_t *regs) {
    debug_log("[ERROR]Double fault, int num: %d", regs->int_num);

    for (;;) {
        halt();
    }
}

/**
 * @brief Недопустимое исключение TSS
 *
 */
static noreturn void invalid_tss() {
    debug_log("[ERROR]Invalid tss");

    for (;;) {
        halt();
    }
}

/**
 * @brief Сегмент недоступен
 *
 */
static noreturn void segment_not_available() {
    debug_log("[ERROR]Segment not available");

    for (;;) {
        halt();
    }
}

/**
 * @brief Ошибка стека
 *
 */
static noreturn void stack_error() {
    debug_log("[ERROR]Stack error");

    for (;;) {
        halt();
    }
}

/**
 * @brief Общая ошибка защиты
 *
 */
static noreturn void general_protection_error(int_registers_t *regs) {
    UNUSED(regs);
    debug_log("[ERROR]GPT error");

    for (;;) {
        halt();
    }
}

/**
 * @brief Ошибка страницы
 *
 */
static noreturn void page_fault(int_registers_t *regs) {
    UNUSED(regs);
    uint32_t addr;
    asm("movl %%cr2, %%eax": "=a"(addr));
    debug_log("[ERROR]Page fault at %x", addr);

    for (;;) {
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

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);

    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);

    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);

    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    // Установка векторов прерываний для ошибок
    int_set_handler(0, &division_by_zero);
    int_set_handler(6, &invalid_opcode);
    int_set_handler(8, &double_fault);
    int_set_handler(10, &invalid_tss);
    int_set_handler(11, &segment_not_available);
    int_set_handler(12, &stack_error);
    int_set_handler(13, &general_protection_error);
    int_set_handler(14, &page_fault);
    int_set_handler(16, &fpu_error);
}
