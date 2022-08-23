#include <stdint.h>

#define SC_CODE_CHANGE_KEYBOARD 55
#define SC_CODE_GET_KEY_LAST    56

/**
 * @brief Установка статуса клавиатуры
 *
 * @params int32_t state - Может принимать 0 / 1 / 2
 */
int32_t changeStageKeyboard(int32_t state){
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_CHANGE_KEYBOARD),
                  "b"(state)
                );
}

/**
 * @brief Возращает последнию нажатую клавишу в течении пару сек
 *
 * @return int32_t - последняя нажатая клавиша
 */
int32_t keyLastInset(){
    int32_t data;
    asm volatile("int $0x80"
                :"=a"(data) : "a"(SC_CODE_GET_KEY_LAST));
    return data;
}
