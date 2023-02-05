/**
 * @file arch.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Архитектурно-зависимые функции
 * @version 0.1.0
 * @date 17-10-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022-2023
 * 
 */


#include <libk.h>


#ifndef    _ARCH_H
#define    _ARCH_H    1

#include <arch/com1_log.h>
#include <arch/tasks.h>


#if (defined __i386__ || defined __x86_64__)


/**
 * @brief Ожидание прерывания
 * 
 */
#define halt() asm volatile("hlt");


/**
 * @brief Запрет прерываний
 * 
 */
#define cli() asm volatile("cli");


/**
 * @brief Разрешение прерываний
 * 
 */
#define sti() asm volatile("sti");


/**
 * @brief Очистка флага направления (DF)
 * 
 */
#define cld() asm volatile("cld")


/**
 * @brief Ядро ОС на 3ГБ выше в ОЗУ
 * 
 */
#define KERNEL_VIRTUAL_OFFSET 0xC0000000


/**
 * @brief Перевод виртуального адреса в физический
 * 
 */
#define V2P(a) ((uintptr_t)(a) & ~KERNEL_OFFSET)


/**
 * @brief Перевод физического адреса в виртуальный
 * 
 */
#define P2V(a) ((uintptr_t)(a) | KERNEL_OFFSET)


/**
 * @brief Физический адрес начала ядра
 * 
 */
extern uintptr_t KERNEL_BEGIN_PHYS;


/**
 * @brief Физический адрес конца ядра
 * 
 */
extern uintptr_t KERNEL_END_PHYS;


/**
 * @brief Виртуальный адрес начала ядра
 * 
 */
extern uintptr_t KERNEL_START;


/**
 * @brief Виртуальный адрес конца ядра
 * 
 */
extern uintptr_t KERNEL_END;


/**
 * @brief Размер ядра
 * 
 */
extern uintptr_t KERNEL_SIZE;

#endif


#if defined(__i386__)


/**
 * @brief Получение значения счётчика TSC
 * 
 * @return unsigned long long Значение счетчика(EAX)
 */
static inline unsigned long long rdtsc() {
    unsigned long long int x;
    asm volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

#elif defined(__x86_64__)


/**
 * @brief Получение значения счётчика TSC
 * 
 * @return unsigned long long Значение счетчика(EAX:EDX)
 */
static inline unsigned long long rdtsc() {
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif

#endif // arch.h