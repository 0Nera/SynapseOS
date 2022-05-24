#include <kernel.h>

task_t tasks[256];
tss_entry_t kernel_tss;
uint8_t tasks_num = 1;

uint8_t current = 0;

void task_switch(struct regs *r){
    //log("task: %d, total tasks: %d, ticks: %d", tasks[current].id, tasks_num, timer_get_ticks());
    tasks_num++;
    // Read esp, ebp now for saving later on.
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
}

// We don't need tss to assist all the task switching, but it's required to have one tss for switching back to kernel mode(system call for example)
void tss_init(uint32_t idx, uint32_t kss, uint32_t kesp) {
    tasks[0].id = 0;
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
    /*kernel_tss.cs = 0x08; // or 0x0b ???
    kernel_tss.ds = 0x10; // or 0x13 ???
    kernel_tss.es = 0x10; // or 0x13 ???
    kernel_tss.fs = 0x10; // or 0x13 ???
    kernel_tss.gs = 0x10; // or 0x13 ???
    kernel_tss.ss = 0x10; // or 0x13 ???*/

    kernel_tss.cs = 0x0b;
    kernel_tss.ds = 0x13;
    kernel_tss.es = 0x13;
    kernel_tss.fs = 0x13;
    kernel_tss.gs = 0x13;
    kernel_tss.ss = 0x13;

    //log("tss now will flush\n");
    //tss_flush();
    //log("tss flushed\n");
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