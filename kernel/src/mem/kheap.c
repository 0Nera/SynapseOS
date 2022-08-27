#include <kernel.h>


virtual_addr_t kheap_begin; // Начало кучи ядра
virtual_addr_t kheap_end; // Конец кучи ядра

uint32_t kheap_memory_used; // Использованно памяти
int32_t kheap_alloc_number; // Число выделений


void kheap_init() {
    kheap_begin = KHEAP_START_VADDR;
    kheap_end = (virtual_addr_t)NULL;

    kheap_alloc_number = 0;
    kheap_memory_used = 0;
    qemu_log("KHEAP installed");
}

uint32_t kheap_getused() {
	return kheap_memory_used;
}

// Увеличение кучи ядра на некоторый размер, он будет округлен до размера страницы
void *kheap_morecore(uint32_t size) {
    if (kheap_end == (virtual_addr_t)NULL) {    // Когда kheap_end == NULL мы должны создать начальную кучу
        kheap_end = kheap_begin;
    }

    void *first_kheap_end = (void*)kheap_end; // Установка адреса возврата

    // Создание страниц
    for (uint32_t pages = (size / PAGE_SIZE) + 1; pages-- > 0; kheap_end += PAGE_SIZE) {
        vmm_alloc_page(kheap_end);
        memset((void*)kheap_end, 0x00, PAGE_SIZE);
    }

    return first_kheap_end; // Мы должны вернуть начальный адрес памяти, которую мы выделили в кучу
}


// Освобождение ранее выделенного элемента из кучи ядра
int32_t kheap_free(void *address) {
    kheap_item *temp_item, *item;

    if (address == NULL) {
        return -1;
    }

    
    item = (kheap_item*) ((uint32_t) address - (uint32_t) sizeof(kheap_item)); // Элемент который нужно освободить

    // Поиск
    for (temp_item = (kheap_item*)kheap_begin; temp_item != NULL; temp_item = temp_item->next) {
        if (temp_item == item) {
            // Освобождение
            temp_item->used = 0;
            kheap_memory_used -= temp_item->size;
            kheap_alloc_number--;

            // Объединение смежных свободных элементов
            for (temp_item = (kheap_item*)kheap_begin; temp_item != NULL; temp_item = temp_item->next) {
                while (!temp_item->used && temp_item->next != NULL && !temp_item->next->used) {
                    temp_item->size += sizeof(kheap_item) + temp_item->next->size;
                    temp_item->next = temp_item->next->next;
                }
            }

            // Конец цикла
            return 1;
        }
    }
    return -1;
}


// Выделение произвольного размера памяти из кучи ядра
void *kheap_malloc(uint32_t size) {
    qemu_log("[Malloc] Request for %d byte space",size);
    kheap_item *new_item = NULL, *temp_item;
    uint32_t total_size;

    if (size == 0) {
        return NULL;
    }

    total_size = ((size + 7) & ~7) + sizeof(kheap_item); // Округляем на 8 байт и добавляем размер заголовка

    kheap_item *last_item;
    // Если куча уже есть
    if (kheap_end != (virtual_addr_t)NULL) {
        // Поиск первого элемента
        for (new_item = (kheap_item*)kheap_begin; new_item != NULL; new_item = new_item->next) {
            if (new_item->next == NULL) {
                last_item = new_item;
            }
            if (!new_item->used && (total_size <= new_item->size)) {
                break;
            }
        }
    }

    // Если 1 найден
    if (new_item != NULL) {
        temp_item = (kheap_item*) ((uint32_t) new_item + total_size);
        temp_item->size = new_item->size - total_size;
        temp_item->used = 0;
        temp_item->next = new_item->next;
    } else {
        // Не нашел подходящего, поэтому мы должны увеличить кучу, чтобы соответствовать
        new_item = kheap_morecore(total_size);
        if (new_item == NULL) {
            // Возвращаем NULL если памяти недостаточно
            return NULL;
        }

        // Создаем пустой элемент для дополнительного пространства (Через kheap_morecore).
        // Мы можем рассчитать размер, потому что kheap_morecore выделяет пространство, выровненное по странице.
        temp_item = (kheap_item*) ((uint32_t) new_item + total_size);
        temp_item->size = PAGE_SIZE - (total_size % PAGE_SIZE 
                                      ? total_size % PAGE_SIZE 
                                      : total_size) - sizeof(kheap_item);
        temp_item->used = 0;
        temp_item->next = NULL;

        last_item->next = new_item;
    }

    // Создание нового элемента
    new_item->size = size;
    new_item->used = 1;
    new_item->next = temp_item;

    kheap_alloc_number++;
    kheap_memory_used += total_size;

    return (void*) ((uint32_t) new_item + (uint32_t) sizeof(kheap_item)); // Возвращаем вновь выделенную ячейку памяти
}
