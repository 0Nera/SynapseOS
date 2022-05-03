#include <kernel.h>


// Holds how many ticks that the system has been running for
int timer_ticks = 0;

void timer_set_frequency(int hz) {
    int divisor = INPUT_CLOCK_FREQUENCY / hz;
    outb(TIMER_COMMAND, TIMER_OCW); // Means Init, Square Wave Mode, non-BCD, first transfer LSB then MSB
    outb(TIMER_DATA, divisor & 0xFF); // Set low byte of divisor
    outb(TIMER_DATA, divisor >> 8); // Set high byte of divisor
    //outb(TIMER_DATA, (divisor >> 8) & 0xFF);
}

// IRQ Handler for the timer. Called at every clock tick
void timer_handler(struct regs *r) {
    timer_ticks++;
    if (1 || timer_ticks % 500 == 0 || r->eax == 13372) {
        qemu_printf("ticks = %d  ", timer_ticks);
        qemu_printf("interrupted context eax = %x  ebx = %x  ecx = %x  edx = %x  esp = %x  ebp = %x  eip = %x  \n", r->eax, r->ebx, r->ecx, r->edx, r->esp, r->ebp, r->eip);
    }
    schedule(r);
}

int timer_get_ticks() {
    return timer_ticks;
}

// Timer init
void timer_install() {
    //register_interrupt_handler(TIMER_IDT_INDEX, task_switch); // was timer_handler
    // set_idt_entry(TIMER_IDT_INDEX, (uint32_t) &task_switch, 0x08, 0x8E);
    // timer_set_frequency(TICKS_PER_SECOND);
}
