/**
 * @file main.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief 
 * @version 0.1.0
 * @date 27-02-2023
 * 
 * @copyright Арен Елчинян (c) 2023
 * 
 */

#include <modlib.h>
#include "module.h"


NAME("Hello World");


/**
 * @brief 
 * 
 */
static module_syscalls_t *s;


/**
 * @brief 
 * 
 * @param input_data 
 * @param input_size 
 */
static void __stdcall handler_default(void *input_data, size_t input_size) {
}


/**
 * @brief 
 * 
 * @param input_data 
 * @param input_size 
 */
static void __stdcall handler_0(void *input_data, size_t input_size) {
}


/**
 * @brief 
 * 
 * @param signal 
 */
static void __stdcall waitress(mod_signal_t *signal) {
    switch (signal->handler) {
        case 0:
            handler_0(signal->input_data, signal->input_size);
            break;
        
        default:
            handler_default(signal->input_data, signal->input_size);
            break;
    }
}


/**
 * @brief 
 * 
 * @param syscalls Сисфункции
 * @param head Структура в которую будет записана информация о модуле
 * @return unsigned int Код ошибки
 */
unsigned int main(module_syscalls_t *syscalls, module_header_t *head) {
    s = syscalls;

    head->waitress    = waitress;
    head->status_code = 1;

    s->printf("Module "__NAME__" loaded with %s", head->cmdline);
    
    return 0;
}
 