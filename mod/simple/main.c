typedef struct {
	void (*printf)(const char *format_string, ...);
	void (*debug_log_printf)(const char *format_string, ...);
} module_syscalls_t;



int _start() {
    return 5;
}