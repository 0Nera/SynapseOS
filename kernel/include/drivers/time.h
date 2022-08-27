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

static const unsigned char synapse_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const unsigned char synapse_months_leap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static const char *months_list[] = {"January", "February", "March", "April", "May", "June", "July",
							"August", "September", "October", "November", "December"};
static const char *months_list_short[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
								  "Sep", "Oct", "Nov", "Dec"};

static const char *weekday_list[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
static const char *weekday_list_short[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void init_task_manager();
void timer_install();
void timer_set_frequency(int32_t hz);
uint32_t timer_get_ticks();
void sleep(uint32_t delay);
void sleep_ms(uint32_t delay);
struct synapse_time get_time();
struct synapse_time* get_time_pointer();
