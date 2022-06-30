#pragma once


#include <stdint.h>
#include <stdbool.h>

// tss входные структуры
typedef struct tss_entry {
    uint32_t prev_tss;

    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;

    uint32_t eip;
    uint32_t eflags;

    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;

    uint32_t esi;
    uint32_t edi;

    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;

    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} tss_entry_t;

typedef uint32_t pid_t;

// Задача
typedef struct {
    list_item_t list_item;      // Элемент списка 
    physical_addres_t  page_dir;// Каталог страниц 
    size_t      threads_count;  // Число потоков в этом процессе 
    bool        suspend;        // Флаг паузы 
    pid_t    pid;            // Идентификатор процесса (PID) 
    char        name[256];      // Имя процесса 

} __attribute__((packed)) process_t;


typedef struct {
    list_item_t list_item;   // Элемент списка 
    process_t*  process;     // Родительский процесс 
    bool        suspend;     // Флаг паузы 
    size_t      stack_size;  // Размер стека потока 
    void*       stack;       // Указатель на блок памяти под стек 
    uint32_t    esp;         // Указатель стека     
    uint32_t    entry_point; // Точка входа в поток 
    pid_t       id;          // Идентификатор потока 
} __attribute__((packed)) thread_t;


extern void tss_flush();
void task_switch();
void tss_init(uint32_t idx, uint32_t kss, uint32_t kesp);