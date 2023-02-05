/**
 * @file tasks.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Новая многозадачность 
 * @version 0.1.0
 * @date 31-01-2023
 * 
 * @copyright Арен Елчинян (c) 2023
 * 
 */


#include <arch.h>
#include <libk.h>
#include <oxygen.h>


#ifndef	_TASKS_H
#define	_TASKS_H	1


/**
 * @brief Регистры получаемые при обработке прерываний
 * 
 */
typedef struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} register_t;


/**
 * @brief ID процесса
 * 
 */
typedef size_t pid_t;


/**
 * @brief Контекст потока
 * 
 */
typedef struct {
    uint32_t eax;   ///< Регистр EAX
    uint32_t ecx;   ///< Регистр ECX
    uint32_t edx;   ///< Регистр EDX
    uint32_t ebx;   ///< Регистр EBX
    uint32_t esp;   ///< Указатель стека
    uint32_t ebp;   ///< Указатель на базу стекового кадра
    uint32_t esi;   ///< Регистр ESI
    uint32_t edi;   ///< Регистр EDI
    uint32_t eflags;///< Регистр флагов, отражает текущее состояние процессора
    uint32_t cr3;   ///< Fдрес каталога таблицы страниц
    uint32_t eip;   ///< Указатель команд
} context_t;


/**
 * @brief Структура процесса
 * 
 */
typedef struct {
    char        name[256];      ///< Имя процесса
    uint8_t     priority;       ///< Приоритет процесса
    pid_t       pid;            ///< Идентификатор процесса (PID)
    list_item_t list_item;      ///< Элемент списка
    uintptr_t   page_dir;       ///< Каталог страниц
    size_t      threads_count;  ///< Число потоков в этом процессе
    uint8_t     status;         ///< Состояние процесса
} process_t;


/**
 * @brief Структура задачи
 * 
 */
typedef struct {
    pid_t       id;          ///< Идентификатор задачи
    uint8_t     priority;    ///< Приоритет задачи
    void        *entry_point;///< Точка входа в задачу
    uint8_t     status;      ///< Состояние задачи
    uintptr_t   esp;         ///< Указатель на стек    
    size_t      stack_size;  ///< Размер стека задачи
    list_item_t list_item;   ///< Элемент списка 
    process_t   *process;    ///< Родительский процесс
} thread_t;


/**
 * @brief Последний сохраненный контекст
 * 
 */
extern register_t saved_context;


/**
 * @brief Первый процесс
 * 
 */
extern process_t *genesis_process;


/**
 * @brief Текущий процесс
 * 
 */
extern process_t *current_process;


/**
 * @brief Функция переключение задачи
 * 
 */
void sheduler_switch();


/**
 * @brief Создать процесс
 * 
 * @param process Указатель на структуру процесса
 * @return true В случае успеха
 * @return false В случае ошибки
 */
bool sheduler_create_process(process_t *process);


/**
 * @brief Создать поток
 * 
 * @param process Указатель на процесс
 * @param entry Входная точка
 * @param priority Приоритет
 * @return thread_t* Указатель на структуру потока
 */
thread_t *sheduler_create_task(process_t *process, void *entry, uint8_t priority);


/**
 * @brief Поток неактивен
 * 
 */
#define THREAD_SLEEP 0


/**
 * @brief Поток стабильно работает
 * 
 */
#define THREAD_NORMAL 1


/**
 * @brief Поток мягкого времени
 * 
 */
#define THREAD_SOFT_TIME 2


/**
 * @brief Поток жесткого времени
 * 
 */
#define THREAD_HARD_TIME 3

#endif // tasks.h
