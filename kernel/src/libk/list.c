/**
 * @file list.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief
 * @version 0.1.0
 * @date 27-09-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <libk.h>

void list_add(list_item_t *list, list_item_t *item) {
    if (item->next == NULL) {
        if (list->next) {
            item->next = item->next;
            item->prev = item->next->prev;
            item->prev->next = item;
            item->next->prev = item;
        } else {
            item->next = item;
            item->prev = item;
            list->next = item;
        }
    }
}

void list_remove(list_item_t *item) {
    if (item->next->next == item) {
        item->next->next = item->next;

        if (item->next->next == item) {
            item->next->next = NULL;
        }
    }

    item->next->prev = item->prev;
    item->prev->next = item->next;
}

void list_init(list_item_t *list) {
    list->next = NULL;
}