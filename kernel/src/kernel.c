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
    UNUSED(kernel_process);
    char name[] = "USER1";
    char hash[65] = "";

    /**
     * @brief Настраиваем указатели на важные функции
     * 
     */
    kernel_size = size;
    kernel_framebuffer = kernel_canvas->framebuffer;
    kprintf = info.prinf;


    debug_log("Ядро SynapseOS запущенно, ядро потребляет: %u килобайт", kernel_size);
    debug_log("%s, %u.%u.%u. [%s]", info.name, info.version.major, info.version.minor, info.version.patch, __TIMESTAMP__);
    debug_log("%ux%u - 0x%x", kernel_canvas->width, kernel_canvas->height, kernel_framebuffer);
    
    kprintf("%s %u.%u.%u based on SynapseOS-core(private) loaded, build: %s. Kernel use: %ukb memory\n",
        info.name, info.version.major, info.version.minor, info.version.patch, __TIMESTAMP__,
        kernel_size);
    
    kprintf("Username: %s\tUID: %s\n", name, hash);
    kprintf("%ux%ux%u - 0x%x", kernel_canvas->width, kernel_canvas->height, kernel_canvas->bpp, kernel_framebuffer);
    kprintf("\tModules loaded: physical %u, virtual: %u, total: %u\n", info.mod_count, info.mod_v_count, info.mod_count + info.mod_v_count);
}