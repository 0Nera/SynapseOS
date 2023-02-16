/**
 * @file scheduler.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief 
 * @version 0.1.0
 * @date 21-12-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


#include <arch.h>
#include <libk.h>


static bool scheduler_busy = true;
static pid_t scheduler_pid_counter = 0;
static uint8_t current_process_priority = 0;
pid_t  next_thread_id = 0;
extern uintptr_t *kernel_page_dir;

list_item_t     process_list;
list_item_t     thread_list;


process_t *kernel_process = 0;
thread_t  *kernel_thread = 0;

process_t *current_process;
thread_t  *current_thread;

/**
 * @brief Инициализация планировщика задач
 * 
 */
bool scheduler_init() {
    uintptr_t esp;
    char *name = "GENESIS"; 


    asm volatile("mov %%esp, %0" : "=r"(esp));

    asm volatile ("cli");
    scheduler_lock();

    list_init(&process_list);
    list_init(&thread_list);

    kernel_process = (process_t*) oxygen_alloc(sizeof(process_t));

    memset(kernel_process, 0, sizeof(process_t));

    kernel_process->pid = scheduler_pid_counter++;
    kernel_process->page_dir = kernel_page_dir;
    kernel_process->threads_count = 1;
    kernel_process->priority = 1;
    strcpy(kernel_process->name, "CORE");

    list_add(&process_list, &kernel_process->list_item);

    kernel_thread = (thread_t*)oxygen_alloc(sizeof(thread_t));

    memset(kernel_thread, 0, sizeof(thread_t));

    kernel_thread->process = kernel_process;
    kernel_thread->id = next_thread_id++;
    kernel_thread->stack_size = 0x4000;
    kernel_thread->esp = esp;
   
    list_add(&thread_list, &kernel_thread->list_item);

    current_process = kernel_process;
    current_thread = kernel_thread;
   
    scheduler_unlock();
   
    asm volatile ("sti");

    return true;
}



/**
 * @brief Смена задачи
 * 
 */
void scheduler_switch() {
    if (scheduler_busy) {
        // Если смена задач заблокированна
    } else {
        asm volatile ("pushf; cli");

        if (current_process_priority--) {
            goto skip;
        }

        asm volatile ("mov %%esp, %0":"=a"(current_thread->esp));

        current_thread = (thread_t*) current_thread->list_item.next;
        current_process_priority = current_thread->priority;

        asm volatile ("mov %0, %%cr3"::"a"(current_process->page_dir));
        asm volatile ("mov %0, %%esp"::"a"(current_thread->esp));

        skip:
        asm volatile ("popf");

    }
}


/**
 * @brief Блокировка смены задач
 * 
 */
void scheduler_lock() {
    scheduler_busy = true;
}


/**
 * @brief Разблокировка смены задач
 * 
 */
void scheduler_unlock() {
    scheduler_busy = false;
}


pid_t scheduler_add_pid() {
    return scheduler_pid_counter++;
}


thread_t *scheduler_create_task(process_t *process,
                        uintptr_t entry_point,
                        uint8_t priority
                        ) {
    uint32_t stack_size = 4096;
    uintptr_t    stack = NULL;
    uintptr_t   eflags;
    
    asm volatile ("pushf; pop %0":"=r"(eflags));
    asm volatile ("cli");

    thread_t* tmp_thread = (thread_t*)oxygen_alloc(sizeof(thread_t));

    memset(tmp_thread, 0, sizeof(thread_t));

    tmp_thread->id = next_thread_id++;
    tmp_thread->process = process;
    tmp_thread->stack_size = stack_size;
    tmp_thread->entry_point = (uintptr_t) entry_point;

    if (priority == 0) {
        tmp_thread->priority = process->priority;
    } else {
        tmp_thread->priority = priority;
    }
    
    stack = oxygen_alloc(stack_size);
   
    tmp_thread->stack = stack;
    tmp_thread->esp = (uintptr_t) stack + stack_size - 12;

    list_add(&thread_list, &tmp_thread->list_item);

    process->threads_count++;


    uintptr_t *esp = (uintptr_t*) (stack + stack_size);


    esp[-1] = (uintptr_t) entry_point;
    esp[-3] = eflags | (1 << 9);
    oxygen_dump_memory();

    asm volatile ("sti");

    return tmp_thread;
}
