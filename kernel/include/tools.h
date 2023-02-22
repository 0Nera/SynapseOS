/**
 * @file tools.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Дополнительные утилиты для упрощения написания кода
 * @version 0.1.0
 * @date 18-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#ifndef _TOOLS_H
#define _TOOLS_H


/**
 * @brief Для неиспользуемых переменных
 *
 */
#define UNUSED(x) (void)(x)


/**
 * @brief Логическое ИЛИ
 *
 */
#define OR ||


/**
 * @brief Логическое И
 *
 */
#define AND &&


/**
 * @brief Инверсия
 *
 */
#define NOT(x) !(x)


/**
 * @brief 
 * 
 * @param x 
 * @return uint32_t 
 */
static inline uint32_t uint24_unpack(uint24_t x) {
    uint32_t result = x.byte[0];
    result = result << 8 | x.byte[1];
    result = result << 8 | x.byte[2];
    return result;
}


/**
 * @brief 
 * 
 * @param x 
 * @return uint24_t 
 */
static inline uint24_t uint24_pack(uint32_t x) {
    uint24_t result;
    result.byte[0] = (x >> 16) & 0xff;
    result.byte[1] = (x >> 8) & 0xff;
    result.byte[2] = x & 0xff;
    return result;
}


#endif