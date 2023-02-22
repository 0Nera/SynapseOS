/**
 * @file com1_log.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции отладки через COM1 порт
 * @version 0.1.0
 * @date 19-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <arch.h>
#include <kernel.h>
#include <libk.h>


#ifndef _COM1_LOG_H
#define _COM1_LOG_H


/**
 * @brief Сохранение области памяти в лог
 *
 * @param addres Адрес области
 * @param size Размер области
 */
void com1_log_dump(void *addres, size_t size);


/**
 * @brief Форматированный вывод в лог
 *
 * @param format_string Форматированная строка
 * @param ... Аргументы
 */
void com1_log_printf(const char *format_string, ...);

#if DEBUG

#if RELEASE
#error "С дуба рухнул? Релиз вместе с дебагом нельзя использовать"
#endif


/**
 * @brief Вывод сообщения в лог
 *
 */
#define com1_log(M, ...)              \
    com1_log_printf("[DEBUG]["        \
                    "%s:"             \
                    "%s:%d] " M "\n", \
        __FILE__,                     \
        __FUNCTION__,                 \
        __LINE__,                     \
        ##__VA_ARGS__)


/**
 * @brief Проверка условия
 *
 */
#define com1_assert(condition)   \
    if (condition) {             \
        com1_log("ASSERT FAIL"); \
        for (;;) {               \
            halt();              \
        }                        \
    }


/**
 * @brief Тест функции
 *
 */
#define com1_unit_test(condition, message)                                                         \
    if ((condition) > 0) {                                                                         \
        com1_log_printf("[TEST PASSED][%s:%s:%u]%s\n", __FILE__, __FUNCTION__, __LINE__, message); \
    } else {                                                                                       \
        com1_log_printf("[TEST FAILED][%s:%s:%u]%s\n", __FILE__, __FUNCTION__, __LINE__, message); \
    }

#else

#if RELEASE


/**
 * @brief Вывод сообщения в лог
 *
 */
#define com1_log(M, ...) \
    com1_log_printf(     \
        M "\n",          \
        ##__VA_ARGS__)
#else


/**
 * @brief Вывод сообщения в лог
 *
 */
#define com1_log(M, ...)  \
    com1_log_printf(      \
        "["               \
        "%s:%d] " M "\n", \
        __FUNCTION__,     \
        __LINE__,         \
        ##__VA_ARGS__)
#endif


/**
 * @brief Проверка условия
 *
 */
#define com1_assert(condition)     \
    if (condition) {               \
        com1_log("[ASSERT FAIL]"); \
        for (;;) {                 \
            halt();                \
        }                          \
    }

#if RELEASE


/**
 * @brief Тест функции
 *
 */
#define com1_unit_test(condition, message)      \
    if ((condition) > 0) {                      \
        com1_log_printf("[PASS]%s\n", message); \
    } else {                                    \
        com1_log_printf("[FAIL]%s\n", message); \
    }
#else

#endif


/**
 * @brief Тест функции
 *
 */
#define com1_unit_test(condition, message)                        \
    if ((condition) > 0) {                                        \
        com1_log_printf("[PASS][%s]%s\n", __FUNCTION__, message); \
    } else {                                                      \
        com1_log_printf("[FAIL][%s]%s\n", __FUNCTION__, message); \
    }
#endif

#endif // com1_log.h
