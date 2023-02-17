/**
 * @file list.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Функции для работы с двусвязными списками
 * @version 0.1.0
 * @date 01-02-2023
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2023
 *
 */

#include <libk.h>

#ifndef LIST_H
#define LIST_H

/**
 * @brief Элемент списка
 *
 */
typedef struct list_item list_item_t;

struct list_item {
    struct list_item* prev; /* Предыдущий элемент списка */
    struct list_item* next; /* Следующий элемент списка */
};

/**
 * @brief Создание списка
 *
 * @param list Указатель на структуру списка
 */
void list_init(list_item_t* list);

/**
 * @brief Добавление элемента в список
 *
 * @param list Элемент списка
 * @param item Элемент который нужно добавить в список
 */
void list_add(list_item_t* list, list_item_t* item);

/**
 * @brief Удаление элемента из списка
 *
 * @param item Элемент списка на удаление
 */
void list_remove(list_item_t* item);

#endif // list.h
