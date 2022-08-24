/**
 * @file apps/libc/tui.c
 * @authors Пиминов Никита (github.com/pimnik98 | VK: @piminov_remont)
 * @brief Дополнительные системные функции
 * @version 0.0.2
 * @date 2022-08-24
 *
 * @copyright Copyright Пиминов Никита (с) 2022
 *
 */
#include <stdint.h>

#define SC_CODE_CHANGE_KEYBOARD 55
#define SC_CODE_GET_KEY_LAST    56
#define SC_CODE_EXEC            59
#define SC_CODE_SHUTDOWN        60
#define SC_CODE_REBOOT          61

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


/**
 * @brief Запустить программу
 *
 * @params const char *name - Полный путь к программе
 * @params int32_t argc     - Кол-во аргументов
 * @params char **argv      - Аргументы
 */
int32_t exec(const char *name, int32_t argc, char **argv){
  int32_t data;
  asm volatile("int $0x80"
                : "=a"(data)
                : "a"(SC_CODE_EXEC),
                  "b"(name),
                  "d"(argc),
                  "c"(argv)
                );
  return data;
}

/**
 * @brief Выключение устройства
 *
 */
void shutdown(){
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_SHUTDOWN)
                );
}

/**
 * @brief Перезагрузка устройства
 *
 */
void reboot(){
    asm volatile("int $0x80"
                :
                : "a"(SC_CODE_SHUTDOWN)
                );
}
