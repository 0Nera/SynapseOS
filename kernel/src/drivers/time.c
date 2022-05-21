#include <kernel.h>



int32_t timer_ticks = 0;

void timer_set_frequency(int32_t hz) {

}


void timer_handler(struct regs *r) {

}

int32_t timer_get_ticks() {
    return timer_ticks;
}

// Timer init
void timer_install() {

}
