/**
 * @file versions.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Типы данных и функции для работы с версиями
 * @version 0.1.0
 * @date 24-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <libk.h>

#ifndef _VERSIONS_H
#define _VERSIONS_H

/**
 * @brief Структура для хранения версий
 *
 */
typedef struct {
    uint16_t major; ///< Версия
    uint16_t minor; ///< Подверсия
    uint16_t patch; ///< Исправление
} version_t;

#endif // string.h