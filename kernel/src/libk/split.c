/**
 * @file kernel.c
 * @author Никита Пиминов (github.com/pimnik98)
 * @brief Содержит функции для работы с split
 * @version 0.0.3
 * @date 2022-08-28
 * @copyright Copyright Piminoff Nikita (c) 2022
 */
#include <kernel.h>

/**
 * @brief Функция откладки
 *
 * @param char* a_str - Строка для деления
 * @param char* del - Делитель (только 1 символ)
 *
 * @return uint32_t - Количество строк
 */
uint32_t str_cdsp(char* a_str, char* del){
    uint32_t x = 0;
    for(int i=0; i < strlen(a_str); i++){
        if (a_str[i] == del[0]) {
            x++;
        }
    }
    return x;
}

/**
 * @brief Функция для деления строк
 *
 * @param char* a_str[] - Строка для деления
 * @param char *out[] - Здесь будет результат работы
 * @param char* del[] - Делитель (только 1 символ)
 *
 * @return char* - Здесь тоже будет результут работы
 */
char* str_split(char a_str[], char *out[], char* dec){
    uint32_t x = str_cdsp(a_str,dec);
    //qemu_log("[SPLIT] [COUNT] %d",x);

    int i = 0;
    char *p;
    p = strtok(a_str, dec);
    if (p == NULL){
        out[i] = a_str;
        return out;
    }
    out[i] = p;
    //qemu_log("[SPLIT] `%s`",out[i]);
    i++;

    for(uint32_t a = 0;a < x; a++){
        p = strtok(out[i], dec);
        if (p != NULL){
            out[i] = p;
            //qemu_log("[SPLIT] [%d < %d] `%s`",i,x,out[i]);
            i++;
        }
    }
    return out;
}
