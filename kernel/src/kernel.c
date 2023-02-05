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


#include <tasks.h>
#include <tools.h>
#include <libk.h>
#include <kernel.h>


/**
 * @brief Размер ядра
 * 
 */
unsigned int kernel_size = 0;


/**
 * @brief Адресс фреймбуффера ядра
 * 
 */
uint32_t *kernel_framebuffer = NULL;

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
void kernel_init(kernel_info_t info, uint32_t size) {
    char str[255];
    struct sha256_buff buff;
    char name[] = "USER1";
    char hash[65] = {0};

    kernel_size = size;
    kernel_framebuffer = kernel_canvas->framebuffer;
    kernel_debug_log = info.debug_log;
    kprinf = info.prinf;


    debug_printf("Ядро SynapseOS запущенно, ядро потребляет: %d килобайт", kernel_size);
    debug_printf("%s, %d.%d.%d. [%s]", info.name, info.version.major, info.version.minor, info.version.patch, __TIMESTAMP__);
    debug_printf("%dx%d - %x", kernel_canvas->width, kernel_canvas->height, kernel_framebuffer);
    //user_print();

    
    sha256_init(&buff);
    sha256_update(&buff, name, 64);
    sha256_update(&buff, __TIMESTAMP__, sizeof(__TIMESTAMP__));
    sha256_finalize(&buff);
    sha256_read_hex(&buff, hash);


    //memset(kernel_framebuffer, '1', 1024 * 768 * 4);
    kprinf("%s %u.%u.%u based on SynapseOS-core(private) loaded, build: %s. Kernel use: %ukb memory\n",
        info.name, info.version.major, info.version.minor, info.version.patch, __TIMESTAMP__,
        kernel_size);
    
    kprinf("Username: %s\tUID: %s\n", name, hash);
    kprinf("%ux%ux%d - %x", kernel_canvas->width, kernel_canvas->height, kernel_canvas->bpp, kernel_framebuffer);
    kprinf("\tModules loaded: physical %u, virtual: %u, total: %u\n", info.mod_count, info.mod_v_count, info.mod_count + info.mod_v_count);
    

    process_t* proc = current_process;
  

    thread_t *thread01, *thread02;
    thread01 = sheduler_create_process(proc,
                &process_checker,
                0x4000,
                1);


    thread02 = sheduler_create_process(proc,
                &pow_worker,
                0x4000,
                1);
}