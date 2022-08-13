#include <kernel.h>


tss_entry_t kernel_tss;

/*
    Многозадачность. 
    Чтобы её реализовать нужно чтобы PIT(timer.c) каждые N единиц времени генерировал прерывание.
    При каждом прерывании от PIT вызывается функция task_switch.
    В task_switch TSS обязан сохранить все регистры текущего процесса и загрузить регмстры следующего процесса.
    Также надо не забывать про приоритет процесса.
*/

uint32_t     next_pid = 0;       // Следующий идентификатор процесса (PID)
uint32_t     next_thread_id = 0; // Следующий идентификатор потока
list_t      process_list;       // Список процессов
list_t     thread_list;        // Список потоков
bool       multi_task = false; // Флаг готовности планировщика
process_t *kernel_proc = 0;    // Описатель процесса ядра
thread_t *kernel_thread = 0;  // Описатель потока ядра
process_t *current_proc;       // Текущий процесс
thread_t *current_thread;     // Текущий поток

/*
    Выделяет номер процессу
*/
static pid_t take_pid() {
    return next_pid++;
}


/*
    Инициализация планировщика задач
*/
void init_task_manager(void) {
    /* Читаем текущий указатель стека */
    uint32_t    esp = 0;
    asm volatile ("mov %%esp, %0":"=a"(esp));       

    /* Выключаем прерывания и инициализируем списки задач */
    asm volatile ("cli");

    list_init(&process_list);
    list_init(&thread_list);

    /* Создаем процесс ядра 
       Выделяем память под описатель процесса и обнуляем её */
    kernel_proc = (process_t*) kheap_malloc(sizeof(process_t));

    memset(kernel_proc, 0, sizeof(process_t));

    /* Инициализируем процесс */
    kernel_proc->pid = take_pid();
    //kernel_proc->page_dir = &kernel_page_dir;
    kernel_proc->list_item.list = NULL;
    kernel_proc->threads_count = 1;
    strcpy(kernel_proc->name, "Kernel");
    kernel_proc->suspend = false;

    //list_add(&process_list, &kernel_proc->list_item);

    /* Делаем процесс и поток ядра текущими */
    current_proc = kernel_proc;
    current_thread = kernel_thread;
   
    /* Взводим флаг готовности планировщика */
    multi_task = true;
   
    /* Снова включаем прерывания */
    asm volatile ("sti");
}


/*
    Выделяет новый процесс
*/
pid_t add_task() {
    return take_pid();
}


/*
    Удаляет процесс
*/
int32_t kill_task(uint32_t pid) {
    return 0;
}


/*
    Переключает задачу
*/
void task_switch() {
	asm volatile ("pushf ");
	asm volatile ("cli");
	asm volatile ("mov %%esp, %0":"=a"(current_thread->esp));

	do{
		current_thread = (thread_t*) current_thread->list_item.next;
		current_proc = (process_t*) current_proc->list_item.next;

	}	while ((current_thread->suspend) || (current_proc->suspend));

	asm volatile ("mov %0, %%esp"::"a"(current_thread->esp));
	asm volatile ("popf");
}



// We don't need tss to assist all the task switching, but it's required to have one tss for switching back to kernel mode(system call for example)
void tss_init(uint32_t idx, uint32_t kss, uint32_t kesp) {
    uint32_t base = (uint32_t) &kernel_tss;
    
    gdt_set_gate(idx, base, base + sizeof(tss_entry_t), /*or 0x89??*/0xE9, 0);

    /* Kernel tss, access(E9 = 1 11 0 1 0 0 1)
        1   present
        11  ring 3
        0   should always be 1, why 0? may be this value doesn't matter at all
        1   code?
        0   can not be executed by ring lower or equal to DPL,
        0   not readable
        1   access bit, always 0, cpu set this to 1 when accessing this sector(why 0 now?)
    */
    memset(&kernel_tss, 0, sizeof(tss_entry_t));
    kernel_tss.ss0 = kss;

    // Note that we usually set tss's esp to 0 when booting our os, however, we need to set it to the real esp when we've switched to usermode because
    // the CPU needs to know what esp to use when usermode app is calling a kernel function(aka system call), that's why we have a function below called tss_set_stack
    kernel_tss.esp0 = kesp;

    kernel_tss.cs = 0x0b;
    kernel_tss.ds = 0x13;
    kernel_tss.es = 0x13;
    kernel_tss.fs = 0x13;
    kernel_tss.gs = 0x13;
    kernel_tss.ss = 0x13;
}

// This function is used to set the tss's esp, so that CPU knows what esp the kernel should be using
void tss_set_stack(uint32_t kss, uint32_t kesp) {
    kernel_tss.ss0 = kss;
    kernel_tss.esp0 = kesp;
}

/*
0xE9 = 11101001
0x89 = 10001001
*/
/*
regs_t *dump_regs(){
    regs_t *tmp;


    asm volatile("mov %%eax, %%cs" 
                : "=a"(tmp->cs)
                );


    return &tmp;
}*/