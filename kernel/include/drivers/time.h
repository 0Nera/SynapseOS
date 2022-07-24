#pragma once

#include <stdint.h>

void init_task_manager();
void timer_install();
void timer_set_frequency();
int32_t timer_get_ticks();
void sleep(uint16_t delay);
