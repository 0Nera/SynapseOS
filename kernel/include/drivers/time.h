#pragma once

#include <stdint.h>

struct synapse_time {
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
	unsigned int day;
	unsigned int month;
	unsigned int year;
	unsigned int century;
};

void init_task_manager();
void timer_install();
void timer_set_frequency();
int32_t timer_get_ticks();
void sleep(uint16_t delay);
struct synapse_time get_time();
