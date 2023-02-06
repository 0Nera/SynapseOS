/**
 * @file kernel.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Главный файл ядра
 * @version 0.3.13
 * @date 17-10-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


#include <tools.h>
#include <libk.h>
#include <kernel.h>


/**
 * @brief Размер ядра
 * 
 */
unsigned int kernel_size = 0;


/**
 * @brief Адрес фреймбуффера ядра
 * 
 */
uint8_t *kernel_framebuffer = NULL;


/**
 * @brief Указатель на настроенный холст ядра
 * 
 */
canvas_t *kernel_canvas;


extern void process_checker();
extern void pow_worker();


/*
    Стоял я молча в стороне
    И пасть готов был на колени,-
    И страшно грустно стало мне,
    Как от присущей милой тени. 
*/


/**
 * @brief Инициализация ядра SynapseOS
 * 
 * @param info Структура с информацией о ядре
 * @param framebuffer Указатель на фреймбуффер экрана
 * @param size Размер ядра в килобайтах
 */
void kernel_init(kernel_info_t info, size_t size, process_t *kernel_process) {
    //struct sha256_buff buff;
    char name[] = "USER1";
    char hash[65] = {0};

    /**
     * @brief Настраиваем указатели на важные функции
     * 
     */
    kernel_size = size;
    kernel_framebuffer = kernel_canvas->framebuffer;
    kprinf = info.prinf;


    debug_log("Ядро SynapseOS запущенно, ядро потребляет: %d килобайт", kernel_size);
    debug_log("%s, %d.%d.%d. [%s]", info.name, info.version.major, info.version.minor, info.version.patch, __TIMESTAMP__);
    debug_log("%dx%d - %x", kernel_canvas->width, kernel_canvas->height, kernel_framebuffer);

    // Это незачем в открытой реализации
    #if 0
    /**
     * @brief Проверка целостности ядра
     * 
     */
    sha256_init(&buff);
    sha256_update(&buff, name, 64);
    sha256_update(&buff, __TIMESTAMP__, sizeof(__TIMESTAMP__));
    sha256_update(&buff, info, sizeof(kernel_info_t));
    sha256_update(&buff, size, sizeof(size_t));
    sha256_finalize(&buff);
    sha256_read_hex(&buff, hash);
    #endif

    
    kprinf("%s %u.%u.%u based on SynapseOS-core(private) loaded, build: %s. Kernel use: %ukb memory\n",
        info.name, info.version.major, info.version.minor, info.version.patch, __TIMESTAMP__,
        kernel_size);
    
    kprinf("Username: %s\tUID: %s\n", name, hash);
    kprinf("%ux%ux%d - %x", kernel_canvas->width, kernel_canvas->height, kernel_canvas->bpp, kernel_framebuffer);
    kprinf("\tModules loaded: physical %u, virtual: %u, total: %u\n", info.mod_count, info.mod_v_count, info.mod_count + info.mod_v_count);
    

    /**
     * @brief Запуск тестовых задач ядра
     * 
     */

    thread_t *thread01 = sheduler_create_task(
        kernel_process,     // Поток привязан к процессу ядра
        &process_checker,   // Функция проверки работы ядра. Выводит справа сверху смайлик
        0                   // Приоритет 0 (1 раз за цикл)
    );


    thread_t *thread02 = sheduler_create_task(
        kernel_process,     // Поток привязан к процессу ядра
        &pow_worker,        // Тестовая нагрузка ядра. 
        1                   // Приоритет (2 раза за цикл)
    );

    thread01->status = THREAD_SLEEP;
    thread02->status = THREAD_SLEEP;

    // Далее возвращаем управление низкоуровневой оболочке
}