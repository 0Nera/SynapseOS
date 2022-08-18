#include <sys/idt.h>


#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_setdev          10
#define SC_CODE_readfile        11
#define SC_CODE_writefile       12
#define SC_CODE_exists          13
#define SC_CODE_filesize        14
#define SC_CODE_rand            20
#define SC_CODE_srand           21
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_getttyinfo      34
#define SC_CODE_version         40
#define SC_CODE_newtask         41
#define SC_CODE_ticks           42
#define SC_CODE_getfreq         43
#define SC_CODE_get_time        44

void syscall_init();
void syscall_handler(struct regs *r);
