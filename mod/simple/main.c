typedef struct {
	void (*printf)(const char *format_string, ...);
	void (*debug_log_printf)(const char *format_string, ...);
} module_syscalls_t;

unsigned int main(module_syscalls_t *syscalls) {
	syscalls->printf("Hello from ELF!\n");
	syscalls->debug_log_printf("Hello from ELF!\n");
    return 5;
}