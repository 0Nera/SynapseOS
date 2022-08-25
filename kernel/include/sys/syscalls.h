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

#define SC_CODE_TTY_POS_X       45
#define SC_CODE_TTY_POS_Y       46
#define SC_CODE_TTY_SCREEN_W    47
#define SC_CODE_TTY_SCREEN_H    48
#define SC_CODE_TTY_SET_COLOR   49
#define SC_CODE_TTY_SET_POS_X   50
#define SC_CODE_TTY_SET_POS_Y   51
#define SC_CODE_TTY_PUT_COLOR   52
#define SC_CODE_TTY_PUTS_COLOR  53
#define SC_CODE_TTY_DRAW_VGA    54
#define SC_CODE_TTY_DRAW_RECT   57

#define SC_CODE_CHANGE_KEYBOARD 55
#define SC_CODE_GET_KEY_LAST    56
#define SC_CODE_CMOS_GET_TIME   58
#define SC_CODE_EXEC            59
#define SC_CODE_SHUTDOWN        60
#define SC_CODE_REBOOT          61

#define SC_CODE_punch           72


void syscall_init();
void syscall_handler(struct regs *r);
