/**
 * @file kernel.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции для работы с ядром
 * @version 0.1.0
 * @date 02-12-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


#include <arch.h>
#include <libk.h>
#include <versions.h>


#ifndef	_KERNEL_H
#define	_KERNEL_H	1


/**
 * @brief Структура для хранения информации о системе
 * 
 */
typedef struct {
    string_utf8_t *name;                                ///< Имя дистрибутива
    version_t version;                                  ///< Версия дистрибутива
    void (*debug_log)(const char *format_string, ...);  ///< Функция для отправки лога
    void (*prinf)(const char *format_string, ...);      ///< Функция для отправки лога
    void *(*malloc)(size_t size);                       ///< Функция для выделения памяти
    void (*free)(void *addr);                           ///< Функция для освобождения памяти
    size_t mod_count;                                   ///< Количество физических модулей
    size_t mod_v_count;                                 ///< Количество виртуальных модулей
} kernel_info_t;


/**
 * @brief Адрес на функцию отладки
 * 
 */
void (*kernel_debug_log)(const char *format_string, ...);


/**
 * @brief Адрес на функцию отладки
 * 
 */
void (*kprinf)(const char *format_string, ...);


#ifdef DEBUG

/**
 * @brief Функция для вывода в лог информации
 * 
 */
#define debug_printf(M, ...)                        \
    kernel_debug_log(                               \
        "[DEBUG]["                                  \
        "%s:"                                       \
        "%s:%d]" M "\n",                            \
        __FILE__,                                   \
        __FUNCTION__,                               \
        __LINE__,                                   \
        ##__VA_ARGS__                               \
        )
#else

/**
 * @brief Функция для вывода в лог информации
 * 
 */
#define debug_printf(M, ...)

#endif  // DEBUG


/**
 * @brief Функция проверки условия
 * 
 */
#define assert(condition) if (condition){                      \
    kernel_debug_log("[ASSERT FAIL]");                                 \
    for(;;) {                                                  \
        halt();                                                \
    }                                                          \
} 


/**
 * @brief Тестирование функций или условий
 * 
 */
#define unit_test(condition, message) if (condition) {            \
    kernel_debug_log("[PASS][%s]%s\n", __FUNCTION__, message);    \
} else {                                                          \
    kernel_debug_log("[FAIL][%s]%s\n", __FUNCTION__, message);    \
}



#endif // kernel.h