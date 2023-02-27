/**
 * @file modlib.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief 
 * @version 0.1.0
 * @date 27-02-2023
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2023
 * 
 */


#include <stdint.h>


#ifndef _MOD_LIB_H
#define _MOD_LIB_H 1

#define NAME(name) \
    const char *__NAME__ = name

 
typedef struct {
    uint32_t     handler;
    uint32_t        code;
    void     *input_data;
    size_t    input_size;
    void         *result;
    size_t   result_size;
} mod_signal_t;


typedef struct {
	void (*printf)(const char *format_string, ...);
	void (*debug_log_printf)(const char *format_string, ...);
} module_syscalls_t;


typedef struct {
    char                          *cmdline;
	unsigned int               status_code;
	unsigned int                      type;
	void (*waitress)(mod_signal_t *signal);
} module_header_t;


#endif // modlib.h
