#pragma once

#include <stddef.h>
#include <stdint.h>


typedef struct list_item list_item_t;


typedef struct {
    list_item_t*    first; // Указатель на первый элемент 
    size_t          count; // Число элементов в списке 
} list_t;


typedef struct list_item {
    list_item_t*    prev; // Предыдущий элемент списка 
    list_item_t*    next; // Следующий элемент списка 
    list_t*         list; // Список которому принадлежит данный элемент 
} list_item_t;


void list_init(list_t* list); //Инициализация списка 
void list_add(list_t* list, list_item_t* item); // Добавление элемента в список 
void list_remove(list_item_t* item); // Удаление элемента из списка 