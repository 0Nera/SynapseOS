#include "../include/time.h"

struct synapse_time* get_time() {
    struct synapse_time* tm;
        asm volatile("int $0x80" 
                :"=a"(tm) : "a"(SC_CODE_get_time));
    return tm;
}