#include <kernel.h>


#define INPUT_CLOCK_FREQUENCY 1193180

#define TIMER_COMMAND 0x43
#define TIMER_DATA    0x40
#define TIMER_OCW     0x36 // Operational command word which means Init, Square Wave Mode, non-BCD, first transfer LSB then MSB

#define TICKS_PER_SECOND 20


uint64_t timer_ticks = 0;
extern void task_switch(void);


void timer_set_frequency(int32_t hz) {
    int divisor = INPUT_CLOCK_FREQUENCY / hz;
    outb(TIMER_COMMAND, TIMER_OCW); // Means Init, Square Wave Mode, non-BCD, first transfer LSB then MSB
    outb(TIMER_DATA, divisor & 0xFF); // Set low byte of divisor
    outb(TIMER_DATA, divisor >> 8); // Set high byte of divisor
    //outb(TIMER_DATA, (divisor >> 8) & 0xFF);
}


void timer_handler(struct regs *r) {
    //log("tick: %d",timer_ticks);
    trand(++timer_ticks);
    //if (multi_task) {
       //task_switch();  /* Переключаемся */
    //}
    // tty_printf("\ntimer_handler idt: %d", r->idt_index);
    //log("timer_handler idt: %d", r->idt_index);
}


int32_t timer_get_ticks() {
    return timer_ticks;
}


// Timer init
void timer_install() {
    register_interrupt_handler(32, &timer_handler);
    IRQ_clear_mask(32 - 32); // - 32 потому что после ремаппинга номера смещаются на 32
    timer_set_frequency(50); // 50 герц
    log("Timer installed");
}

void sleep(uint16_t delay)
{
    uint64_t current_ticks = timer_get_ticks();
    while (1)
    {
        if (current_ticks + delay < timer_get_ticks()){
            break;
        }
    }
}

