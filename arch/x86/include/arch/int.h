/**
 * @file int.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции обработки прерываний
 * @version 0.1.0
 * @date 21-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#ifndef _INT_H
#define _INT_H 1

/**
 * @brief Максимальное количество векторов прерываний (1023 + 1)
 *
 */
#define INTERRUPT_MAX_INDEX 1023

/**
 * @brief Программируемый интервальный таймер (высокоточный таймер событий 0)
 *
 */
#define IRQ0 32

/**
 * @brief Клавиатура PS/2
 *
 */
#define IRQ1 33

/**
 * @brief Запрос прерывания от ведомого контролера (каскадирование)
 *
 */
#define IRQ2 34

/**
 * @brief Произвольное устройство (IBM PC/AT - последовательный порт)
 *
 */
#define IRQ3 35

/**
 * @brief Произвольное устройство (IBM PC/AT - последовательный порт)
 *
 */
#define IRQ4 36

/**
 * @brief Произвольное устройство (IBM PC/AT - параллельный порт)
 *
 */
#define IRQ5 37

/**
 * @brief Произвольное устройство (IBM PC/AT - контроллер FDD)
 *
 */
#define IRQ6 38

/**
 * @brief Произвольное устройство (IBM PC/AT - параллельный порт)
 *
 */
#define IRQ7 39

/**
 * @brief Часы реального времени (высокоточный таймер событий 1)
 *
 */
#define IRQ8 40

/**
 * @brief Произвольное устройство
 *
 */
#define IRQ9 41

/**
 * @brief Произвольное устройство
 *
 */
#define IRQ10 42

/**
 * @brief Произвольное устройство или высокоточный таймер событий 2
 *
 */
#define IRQ11 43

/**
 * @brief Произвольное устройство, мышь PS/2 или высокоточный таймер событий 3
 *
 */
#define IRQ12 44

/**
 * @brief Ошибка FPU
 *
 */
#define IRQ13 45

/**
 * @brief Произвольное устройство, первый контролер ATA (или контролер SATA в режиме совместимости)
 *
 */
#define IRQ14 46

/**
 * @brief Произвольное устройство, второй контролер ATA (или контролер SATA в режиме совместимости)
 *
 */
#define IRQ15 47

/**
 * @brief Структура для хранения регистров
 *
 */
struct int_regs {
    uint32_t ds; ///< Регистр
    uint32_t edi; ///< Регистр
    uint32_t esi; ///< Регистр
    uint32_t ebp; ///< Регистр
    uint32_t esp; ///< Регистр
    uint32_t ebx; ///< Регистр
    uint32_t edx; ///< Регистр
    uint32_t esx; ///< Регистр
    uint32_t eax; ///< Регистр
    uint32_t int_num; ///< Номер прерывания
    uint32_t err_code; ///<  Код ошибки
    uint32_t eip; ///< Регистр
    uint32_t cs; ///< Регистр
    uint32_t eflags; ///< Регистр
    uint32_t useresp; ///< Регистр
    uint32_t ss; ///< Регистр
} PACKED; ///< Структура запакованна

/**
 * @brief Тип данных для хранения регистров
 *
 */
typedef struct int_regs int_registers_t;

bool int_set_handler(uint16_t index, void* func);
void int_init();

/**
 * @brief Тип даннных для указателя-функции обработчика прерываний
 *
 */
typedef void (*isr_t)(int_registers_t);

/* Регистрация обработчика прерывания */
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif // int.h