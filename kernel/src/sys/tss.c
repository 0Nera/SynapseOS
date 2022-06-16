#include <kernel.h>

task_t tasks[256];
tss_entry_t kernel_tss;
uint32_t tasks_count = 1;
uint32_t tasks_num = 1;
static uint8_t current = 0;
uint32_t new_task_pid = 0;


/*
    Многозадачность. 
    Чтобы её реализовать нужно чтобы PIT(timer.c) каждые N единиц времени генерировал прерывание.
    При каждом прерывании от PIT вызывается функция task_switch.
    В task_switch TSS обязан сохранить все регистры текущего процесса и загрузить регмистры следующего процесса.
    Также надо не забывать про приоритет процесса.
*/

void task_switch(struct regs *r){
    int old = current;      // Старый таск = новый

    log("1 current task: %d, next: %d, tasks_count: %d, pid: %d", current, tasks[current].next, tasks_count, tasks[current].pid);
    tasks[old].r = &r;

    current = tasks[old].next;

    r = &tasks[current].r;
    
    log("2 current task: %d, next: %d, tasks_count: %d, pid: %d", current, tasks[current].next, tasks_count, tasks[current].pid);
}


int create_task(){
    log("Creating task...");
    int result = 0;

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_newtask)
                );

    log("result %d", result);
    return result;
}

_Noreturn kill_task(uint32_t pid){
    asm volatile("cli");
    int cur = -1;
    for (int i = 0; i < 255; i++){
        if(tasks[i].pid == pid & cur == -1){
            cur = i;
            log("Find! %d", i);
            i = 0;
        }

        if(cur > 0 & tasks[i].next == pid){
            tasks[i].next = tasks[cur].next;
            log("tasks[%d].pid = %d", i, tasks[i].pid);
            break;
        }
    }
    asm volatile("sti");
    for(;;){
        asm volatile("nop");
    }
}


int add_task(struct regs *r){
    uint32_t adr;
    asm volatile("movl %%cr2, %0" : "=r" (adr));

    new_task_pid = tasks_count * 2;                 // pid новой задачи = количество задач

    log("Task pid: %d, tasks_num - 1: %d", new_task_pid, tasks_num - 1);

    tasks[tasks_num].r = &r;                   // Сохраняем регистры
    tasks[tasks_num].pid = new_task_pid;       // Сохраняем pid
    tasks[tasks_num].next = 0;                 // Следующая задача - ядро

    if (tasks_num > 0){
        log("last task data:\n"\
            "pid: %d\n" \
            "next: %d", tasks[tasks_num - 1].pid, tasks[tasks_num - 1].next
            );
        
        tasks[tasks_num - 1].next = new_task_pid;  // Прошлая задача.pid = pid текущей задачи
    }
    log("task data:\n"\
        "pid: %d\n" \
        "next: %d", tasks[tasks_num].pid, tasks[tasks_num].next
        );
    tasks_count++;                              // Количество задач увеличиваем
    tasks_num++;

    log("Task created");
    return new_task_pid;
}


// We don't need tss to assist all the task switching, but it's required to have one tss for switching back to kernel mode(system call for example)
void tss_init(uint32_t idx, uint32_t kss, uint32_t kesp) {
    //tasks[tasks_num].r = &r;                   // Сохраняем регистры
    tasks[0].pid = 0;       // Сохраняем pid
    tasks[0].next = 0;                 // Следующая задача - ядро
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