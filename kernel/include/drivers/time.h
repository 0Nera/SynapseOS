#pragma once

#include <stdint.h>

struct synapse_time {
	int seconds;
	int minutes;
	int hours;
	int day;
	int month;
	int year;
	int century;
};

void init_task_manager();
void timer_install();
void timer_set_frequency();
int32_t timer_get_ticks();
void sleep(uint16_t delay);
struct synapse_time get_time();
