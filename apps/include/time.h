#ifndef TIME_H_
#define TIME_H_

#define SC_CODE_get_time        44

struct synapse_time {
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
	unsigned int day;
	unsigned int month;
	unsigned int year;
	unsigned int century;
};

struct synapse_time* get_time();

#endif