#include <kernel.h>


uint32_t sc_puts(char *str) {
    tty_printf("%s", str);
    return (uint32_t)100;
}


uint32_t sc_getscancode() {
    return (uint32_t)keyboard_getscancode();
}


uint32_t sc_getchar() {
    return (uint32_t)keyboard_getchar();
}


uint32_t sc_gets() {
    return keyboard_gets();
}


uint32_t sc_putpixel(int x, int y, uint32_t color) {
    set_pixel(x, y, color);
    return (uint32_t)110;
}


uint32_t sc_drawline(int x, int y, int xe, int ye, uint32_t color){
    set_line(x, y, xe, ye, color);
    return (uint32_t)111;
}


uint32_t* malloc(int size){
    return (uint32_t*)kheap_malloc(size);
}


uint32_t free(void* addr){
    kheap_free(addr);
    return (uint32_t)113;
}


void syscall_init() {
    register_interrupt_handler(SYSCALL_IDT_INDEX, &syscall_handler);
    qemu_putstring("SYSCALL INIT\n");
}

#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_version         40

void syscall_handler(struct regs *r) {
    uint32_t result = -1;
    uint32_t* argptr = (uint32_t*) (r->ebx);
    /*uint32_t adr;
    asm volatile("movl %%cr2, %0" : "=r" (adr));

    tty_printf("\nsyscall_handler cr2 = %d  r->idt_index = %d eax = %d  ebx = %d" \
        "  ecx = %d  edx = %d  esp = %d  ebp = %d  eip = %d\n", 
        adr, r->idt_index, r->eax, r->ebx, 
        r->ecx, r->edx, r->esp, r->ebp, r->eip);*/
    switch (r->eax) {
        case SC_CODE_puts:
            //qemu_printf("str = %x\n", (char*) (argptr[0]));
            result = sc_puts((char*) (argptr[0]));
            break;
        case SC_CODE_getscancode:
            result = sc_getscancode();
            qemu_printf("input eax: %d result = %d, [%c] [%s]\n", r->eax, result, result, result);
            break;
        case SC_CODE_getchar:
            result = sc_getchar();
            break;
        case SC_CODE_gets:
            result = sc_gets();
            break;
        case SC_CODE_malloc:
            result = malloc((int)argptr[0]);
            break;
        case SC_CODE_free:
            free(argptr[0]);
            break;
        case SC_CODE_putpixel:
            result = sc_putpixel((int) (argptr[0]), (int) (argptr[1]), (uint32_t)(argptr[2]));
            break;
        case SC_CODE_drawline:
            result = sc_drawline((int) (argptr[0]), (int) (argptr[1]),(int) (argptr[2]), (int) (argptr[3]), (uint32_t) (argptr[4]));
            break;
        case SC_CODE_version:
            result = 26;
            break;
        default: 
            qemu_printf("Invalid syscall #%x\n", r->eax);
            tty_printf("Invalid syscall #%x\n", r->eax);
    }


    r->edx = (uint32_t)result;
}
