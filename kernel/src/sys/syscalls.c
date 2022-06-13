/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/


#include <kernel.h>


void syscall_init() {
    register_interrupt_handler(SYSCALL_IDT_INDEX, &syscall_handler);
    log("Syscalls enabled");
}


void syscall_handler(struct regs *r) {
    /*
        Регистры сисфункций:
        eax - номер сисфункции
        ebx - параметр-указатель 1
        edx - параметр-указатель 2
        ecx - параметр-указатель 3
        esi - параметр-указатель 4
        edi - параметр-указатель 5
        ebp - параметр-указатель 6
    */
   
    uint32_t* argptr1 = (uint32_t*) (r->ebx);
    uint32_t* argptr2 = (uint32_t*) (r->edx);
    uint32_t* argptr3 = (uint32_t*) (r->ecx);
    uint32_t* argptr4 = (uint32_t*) (r->esi);
    uint32_t* argptr5 = (uint32_t*) (r->edi);
    uint32_t* argptr6 = (uint32_t*) (r->ebp);

    switch (r->eax) {
        case SC_CODE_puts:                  // I/O
            tty_printf("%s", (char*) (argptr1));
            r->ebp = (uint32_t)1;
            break;
        case SC_CODE_getscancode:
            r->ebp = (uint32_t)keyboard_getscancode();
            break;
        case SC_CODE_getchar:
            r->ebp = (uint32_t)keyboard_getchar();
            break;
        case SC_CODE_gets:
            r->ebp = (uint32_t)keyboard_gets();
            break;
        case SC_CODE_malloc:
            r->ebp = (uint32_t)kheap_malloc((int)argptr1);
            break;
        case SC_CODE_free:
            kheap_free((void*)argptr1);
            r->ebp = (uint32_t)1;
            break;
        case SC_CODE_setdev:                // Хранилище
            r->ebp = (uint32_t)SSFS_set_device((int)argptr1);
            break;
        case SC_CODE_readfile:
            r->ebp = (uint32_t)vfs_read((char *)argptr1, (int32_t)argptr2, (int32_t)argptr3, (void *)argptr4);
            break;
        case SC_CODE_putpixel:              // Графика
            set_pixel((int) (argptr1), (int) (argptr2), (uint32_t)(argptr3));
            r->ebp = (uint32_t)1;
            break;
        case SC_CODE_drawline:
            set_line((int) (argptr1), (int) (argptr2),(int) (argptr3), (int) (argptr4), (uint32_t) (argptr5));
            r->ebp = (uint32_t)1;
            break;
        case SC_CODE_version:               // Система
            r->ebp = (uint32_t)(VERSION_MAJOR * 100 + VERSION_MINOR);
            break;
        default: 
            log("Invalid syscall #%x", r->eax);
            log("r->idt_index = %x eax = %x  ebx = %x  " \
            "ecx = %x  edx = %x  esp = %x  ebp = %x  eip = %x", 
             r->idt_index, r->eax, r->ebx, 
            r->ecx, r->edx, r->esp, r->ebp, r->eip);
            tty_printf("Invalid syscall #%x\n", r->eax);
    }
}