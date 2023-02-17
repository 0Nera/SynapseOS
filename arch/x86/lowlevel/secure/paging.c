/**
 * @file paging.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Страничная адресация
 * @version 0.1.0
 * @date 06-02-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <libk.h>

#define KERNEL_BASE 0x200000 // Адрес ядра             (2мб)
#define KERNEL_BASE_END KERNEL_BASE + 0x300000 // Адрес конца ядра       (5мб),
                                               // в сумме 3мб на файл ядра
#define KERNEL_END_ADDR KERNEL_BASE + 0xE00000 // Размер памяти ядра      (16мб)
#define KERNEL_PAGE_TABLE KERNEL_BASE_END // Адрес страничной памяти (5мб)
#define KERNEL_PAGE_TABLE_END KERNEL_PAGE_TABLE + 0x400000 // Конец страничной памяти (9мб),
                                                           // в сумме 4мб на страницы
uintptr_t* kernel_page_dir = (uintptr_t*)KERNEL_PAGE_TABLE;

void paging_init()
{
}