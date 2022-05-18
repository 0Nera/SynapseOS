/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/


#include <kernel.h>


void syscall_init() {
    register_interrupt_handler(SYSCALL_IDT_INDEX, &syscall_handler);
    qemu_printf("SYSCALL INIT\n");
}


void syscall_handler(struct regs *r) {
    uint32_t* argptr = (uint32_t*) (r->ebx);
    switch (r->eax) {
        case SC_CODE_puts:                  // I/O
            tty_printf("%s", (char*) (argptr[0]));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_getscancode:
            r->edx = (uint32_t)keyboard_getscancode();
            break;
        case SC_CODE_getchar:
            r->edx = (uint32_t)keyboard_getchar();
            break;
        case SC_CODE_gets:
            r->edx = (uint32_t)keyboard_gets();
            break;
        case SC_CODE_malloc:
            r->edx = (uint32_t)kheap_malloc((int)argptr[0]);
            break;
        case SC_CODE_free:
            kheap_free((void*)argptr[0]);
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_setdev:                // Хранилище
            r->edx = (uint32_t)SSFS_set_device((int)argptr[0]);
            break;
        case SC_CODE_readfile:
            SSFS_read((argptr[0]),(argptr[1]));
            r->edx = (uint32_t)0;
            break;
        case SC_CODE_putpixel:              // Графика
            set_pixel((int) (argptr[0]), (int) (argptr[1]), (uint32_t)(argptr[2]));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_drawline:
            set_line((int) (argptr[0]), (int) (argptr[1]),(int) (argptr[2]), (int) (argptr[3]), (uint32_t) (argptr[4]));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_version:               // Система
            r->edx = (uint32_t)(VERSION_MAJOR * 100 + VERSION_MINOR);
            break;
        default: 
            qemu_printf("Invalid syscall #%x\n", r->eax);
            tty_printf("Invalid syscall #%x\n", r->eax);
    }
}