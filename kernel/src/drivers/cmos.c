/**
 * @file cmos.c
 * @brief Драйвер CMOS
 * @author Арен Елчинян (a2.dev@yandex.com), Андрей(Давид) Павленко (andrejpavlenko666@gmail.com)
 */
 
#include <kernel.h>

#define CURRENT_YEAR        2022    // Change this each year!
 
int32_t century_register = 0x00;     // Set by ACPI table parsing code if possible
 
unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
uint32_t year;
unsigned char century;

unsigned char last_second;
unsigned char last_minute;
unsigned char last_hour;
unsigned char last_day;
unsigned char last_month;
unsigned char last_year;
unsigned char last_century;
unsigned char registerB;

enum {
    cmos_address = 0x70,
    cmos_data    = 0x71
};

/**
 * @brief Проверяет CMOS на обновление
 */
int32_t get_update_in_progress_flag() {
    outb(cmos_address, 0x0A);
    return (inb(cmos_data) & 0x80);
}

/**
 * @brief Получает регистр CMOS
 */
unsigned char get_RTC_register(int32_t reg) {
    outb(cmos_address, reg);
    return inb(cmos_data);
}

/**
 * @brief Считывает время с CMOS
 */
 
void read_rtc() {
    while (get_update_in_progress_flag());          // Make sure an update isn't in progress
    second = get_RTC_register(0x00);
    minute = get_RTC_register(0x02);
    hour = get_RTC_register(0x04);
    day = get_RTC_register(0x07);
    month = get_RTC_register(0x08);
    year = get_RTC_register(0x09);
    if(century_register != 0) {
        century = get_RTC_register(century_register);
    }
 
    do {
        last_second = second;
        last_minute = minute;
        last_hour = hour;
        last_day = day;
        last_month = month;
        last_year = year;
        last_century = century;
 
        while (get_update_in_progress_flag());       // Make sure an update isn't in progress
        second = get_RTC_register(0x00);
        minute = get_RTC_register(0x02);
        hour = get_RTC_register(0x04);
        day = get_RTC_register(0x07);
        month = get_RTC_register(0x08);
        year = get_RTC_register(0x09);
        if(century_register != 0) {
            century = get_RTC_register(century_register);
        }
    } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
           (last_day != day) || (last_month != month) || (last_year != year) ||
           (last_century != century) );
 
    registerB = get_RTC_register(0x0B);
 
    // Convert BCD to binary values if necessary
 
    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
        if(century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }
 
    // Convert 12 hour clock to 24 hour clock if necessary
 
    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }
 
    // Calculate the full (4-digit) year
 
    if(century_register != 0) {
        year += century * 100;
    } else {
        year += (CURRENT_YEAR / 100) * 100;
        if(year < CURRENT_YEAR) year += 100;
    }
}

/**
 * @brief Это високосный год?
 */
int isleap(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

/**
 * @brief Считывает время и передает в удобной структуре
 */
struct synapse_time get_time() {
    read_rtc();
	struct synapse_time time = {
		second, minute, hour, day, month, year, century
	};
	return time;
}

struct synapse_time* get_time_pointer() {
    read_rtc();
	struct synapse_time* time = NULL;
	time->seconds = second;
    time->minutes = minute;
    time->hours = hour;
    time->day = day;
    time->month = month;
    time->year = year;
    time->century = century;
	return time;
}

/*
unsigned int synapse_time_to_unix(struct synapse_time ktime) {
	unsigned int t = 0;
	unsigned char cmdt = (isleap(ktime.year)?synapse_months_leap[ktime.month-1]:synapse_months[ktime.month-1]);

	t += ktime.seconds;
	t += ktime.minutes*60;
	t += ktime.hours*60*60;
	t += ktime.day*60*60*24;
	t += ktime.month*60*60*24*cmdt;
	t -= 33555600; // WHAT A MAGIC???
	t += (ktime.year-1970)*60*60*24*cmdt*12;
    return t;
}
*/
