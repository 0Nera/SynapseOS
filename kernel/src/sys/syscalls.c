/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/

#include <kernel.h>


static uint64_t next_rnd = 1;


void trand(uint32_t time) {
    next_rnd = next_rnd * 1103515245 + 12345 + time;
}


uint64_t rand(void) {
    next_rnd = next_rnd * 1103515245 + 12345;
    return (uint64_t)(next_rnd/65536) % 32768;
}


void srand(uint32_t seed) {
    next_rnd = seed;
}


void syscall_init() {
    register_interrupt_handler(SYSCALL_IDT_INDEX, &syscall_handler);
    log("Syscalls enabled");
}


void syscall_handler(struct regs *r) {
    /*
        Регистры сисфункций:
        eax - номер сисфункции и результат
        ebx - параметр-указатель 1
        edx - параметр-указатель 2
        ecx - параметр-указатель 3
        esi - параметр-указатель 4
        edi - параметр-указатель 5
    */

    uint32_t arg1 = r->ebx;
    uint32_t arg2 = r->edx;
    uint32_t arg3 = r->ecx;
    uint32_t arg4 = r->esi;
    uint32_t arg5 = r->edi;

    switch (r->eax) {
        case SC_CODE_puts: // I/O
            tty_printf("%s", (char *)(arg1));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_getscancode:
            r->eax = (uint32_t)keyboard_getscancode();
            break;
        case SC_CODE_getchar:
            r->eax = (uint32_t)keyboard_getchar();
            break;
        case SC_CODE_gets:
            r->eax = (uint32_t)keyboard_gets();
            break;
        case SC_CODE_malloc:
            r->eax = (uint32_t)kheap_malloc((int32_t)arg1);
            break;
        case SC_CODE_free:
            kheap_free((void *)arg1);
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_setdev: // Хранилище
            r->eax = (uint32_t)-1;
            break;
        case SC_CODE_readfile:
            r->eax = (uint32_t)vfs_read((char *)arg1, (int32_t)arg2, (int32_t)arg3, (void *)arg4);
            break;
        case SC_CODE_rand:
            r->eax = (uint64_t)rand();
            break;
        case SC_CODE_srand:
            srand((uint64_t)(arg1));
            r->eax = (uint64_t)rand();
            break;
        case SC_CODE_putpixel: // Графика
            set_pixel((int32_t)(arg1), (int32_t)(arg2), (uint32_t)(arg3));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_drawline:
            set_line((int32_t)(arg1), (int32_t)(arg2), (int32_t)(arg3), (int32_t)(arg4), (uint32_t)(arg5));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_version: // Система
            r->eax = (uint32_t)(VERSION_MAJOR * 100 + VERSION_MINOR);
            break;
        case SC_CODE_newtask: 
            r->eax = add_task(&r);
            break;
        default:
            log("Invalid syscall #%x", r->eax);
            log("r->idt_index = %x eax = %x  ebx = %x  "
                "ecx = %x  edx = %x  esp = %x  ebp = %x  eip = %x",
                r->idt_index, r->eax, r->ebx,
                r->ecx, r->edx, r->esp, r->ebp, r->eip);
            tty_printf("Invalid syscall %x\n", r->eax);
            tty_printf("Invalid syscall %d\n", r->eax);
            tty_printf("r->idt_index = %x eax = %x  ebx = %x  "
                    "ecx = %x  edx = %x  esp = %x  ebp = %x  eip = %x",
                    r->idt_index, r->eax, r->ebx,
                    r->ecx, r->edx, r->esp, r->ebp, r->eip);
            tty_printf("r->idt_index = %d eax = %d  ebx = %d  "
                    "ecx = %d  edx = %d  esp = %d  ebp = %d  eip = %d",
                    r->idt_index, r->eax, r->ebx,
                    r->ecx, r->edx, r->esp, r->ebp, r->eip);
    }
}