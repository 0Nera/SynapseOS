/**
 * @file init.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Инициализация ядра на i686 архитектуре
 * @version 0.1.0
 * @date 06-02-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022
 * 
 */


#include <arch.h>       // Архитектурнозависимые функции
#include <mm/oxygen.h>  // Менеджер памяти oxygen
#include <graf.h>  // Менеджер памяти oxygen
#include <kernel.h>     // Информация о системе и функции ОС
#include <libk.h>       // Библиотека ядра
#include <versions.h>   // Версионирование
#include <multiboot.h>  // Multiboot1


void *i686_malloc(size_t size) {
    return oxygen_alloc(size);
}


void i686_free(void *addr) {
    oxygen_free(addr);
}

extern canvas_t *kernel_canvas;

// Информация о дистрибутиве
kernel_info_t kernel_info = {
    "SynapseOS core", 
    {
        0, 3, 18
    },
    &debug_log_printf,
    &text_printf,
    &i686_malloc,
    &i686_free,
    0,
    0
};

extern void kernel_init(kernel_info_t info, uint32_t size);



static inline uint32_t cpu_get_features() {
    uint32_t edx;

    asm volatile(
                "movl $0x1, %%eax   \n\t"
                "cpuid              \n\t"
                "movl %%edx, %0     \n\t"
                : "=g"(edx)
                :
                : "%eax", "%ebx", "%ecx", "%edx");
        
    return edx;
}


uint8_t *multiboot_framebuffer = NULL; ///< Адресс фреймбуффера полученный от загрузчика
uint32_t multiboot_framebuffer_width = 0;
uint32_t multiboot_framebuffer_height = 0;
uint32_t multiboot_framebuffer_pitch = 0;
uint32_t multiboot_framebuffer_bpp = 0;
bool pit_init(uint32_t frequency);
extern void *ui_module;
/**
 * @brief Функция инициализации i386 ядра
 * 
 * @param eax Магическое число
 * @param ebx Указатель на данные загрузчика
 * @param esp Стек
 */
void kernel_startup(unsigned int eax, unsigned int ebx, unsigned int esp) {
    debug_log_printf = com1_log_printf;
    UNUSED(esp);
    
    ports_outb(PORTS_COM1 + 1, 0x00);
    ports_outb(PORTS_COM1 + 3, 0x80);
    ports_outb(PORTS_COM1 + 0, 0x03);
    ports_outb(PORTS_COM1 + 1, 0x00);
    ports_outb(PORTS_COM1 + 3, 0x03);
    ports_outb(PORTS_COM1 + 2, 0xC7);
    ports_outb(PORTS_COM1 + 4, 0x0B);
    
    debug_log("Структура multiboot %x", ebx);

    uint32_t cpu = cpu_get_features();

    if ((cpu >> 23) & 0x1) {
        debug_log("Имеется MMU");
        oxygen_multiboot_init(ebx);
        paging_init();
    } else {
        debug_log("Процессоры без MMU не поддерживаются!");
        
        for (;;) {
            halt();
        }
    }

    // ESP пока не используем, стек позже пригодится при переходе в ring3
    //UNUSED(esp);

    int kernel_size = ((uint32_t)&KERNEL_SIZE) >> 10;  // Размер ядра делим на 1024, получая размер в килобайтах.
    debug_log("Размер ядра(вместе со стеком) %u килобайт, %u байт", kernel_size, (uint32_t)&KERNEL_SIZE);

    com1_unit_test(eax == MULTIBOOT_BOOTLOADER_MAGIC, "Проверка магического числа Multiboot");
    
    
    multiboot_main(ebx);
    
    //vesa_init();                                      - не реализовано в открытом ядре

    //mm_evelina_multiboot_init(ebx, 8192);
    

    debug_log("cpu: %x", cpu);
    //mm_init(0x100000, 0x3EE0000);

    
    com1_unit_test(dt_init(), "Настройка таблиц дескрипторов");

    //com1_unit_test(pmm_init(), "Настройка менеджера физической памяти");

    //com1_unit_test(kheap_init((uint8_t*)(0x1000000)), "Инициализация кучи ядра");
    //com1_unit_test(kheap_init((uint8_t*)(&KERNEL_END)), "Инициализация кучи ядра");
    //arch_cpuid_test();

    debug_log("cpu: %x", cpu);

    if (cpu & 0x1) {
        debug_log("Имеется FPU");
        
        
        com1_unit_test(fpu_init(), "Настройка FPU");
    }

    if ((cpu >> 25) & 0x1) {
        debug_log("Имеется SSE");
    }
    
    vesa_init(ebx);

    kernel_canvas = oxygen_alloc(sizeof(canvas_t));

    kernel_canvas->width = multiboot_framebuffer_width;
    kernel_canvas->height = multiboot_framebuffer_height;
    kernel_canvas->pitch = multiboot_framebuffer_pitch;
    kernel_canvas->bpp = multiboot_framebuffer_bpp;
    kernel_canvas->framebuffer = multiboot_framebuffer;
    
    com1_unit_test(sheduler_init(), "Настройка планировщика задач");
    
    text_init();

    graf_install();

    sti();
    
    com1_unit_test(pit_init(100), "Установка PIT");

    
    // Вызываем ядро SynapseOS

    kernel_init(kernel_info, kernel_size);

    debug_log("Ядро завершило инициализацию");

    // Уменьшаем энергопотребление процессора
    for (;;) {
        halt();
    }
}


/*
    Человеку нужен человек,
    Позвонить ему, послушать голос:
    «А у нас сегодня выпал снег.
    Как ты без меня там? Беспокоюсь!»

    Чтобы был приятель, друг, сосед,
    И еще сопящая под боком,
    Без которой счастья в жизни нет,
    Без которой очень одиноко…
*/