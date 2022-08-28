/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/
/**
 * @file kernel/src/sys/syscalls.c
 * @authors Арен Елчинян (a2.dev@yandex.com), Пиминов Никита (github.com/pimnik98 | VK: @piminov_remont)
 * @brief Прослойка для системных вызовов
 * @version 0.0.2
 * @date 2022-08-23
 *
 * @copyright Copyright SynapseOS Team (с) 2022
 *
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
    qemu_log("Syscalls enabled");
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
            //tty_printf("%s", (char *)(arg1));
            _tty_puts((char*)arg1);
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_TTY_DRAW_RECT:
            drawRect((int)(arg1),(int)(arg2),(int)(arg3),(int)(arg4),(int)(arg5));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_CHANGE_KEYBOARD:
            changeStageKeyboard((int32_t)(arg1));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_GET_KEY_LAST:
            r->eax = (uint32_t)keyLastInset();
            break;
        case SC_CODE_TTY_POS_X:
            r->eax = (uint32_t)getPosX();
            break;
        case SC_CODE_TTY_POS_Y:
            r->eax = (uint32_t)getPosY();
            break;
        case SC_CODE_TTY_SCREEN_W:
            r->eax = (uint32_t)getWidthScreen();
            break;
        case SC_CODE_TTY_SCREEN_H:
            r->eax = (uint32_t)getHeightScreen();
            break;
        case SC_CODE_TTY_SET_COLOR:
            tty_setcolor((int32_t)(arg1));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_TTY_SET_POS_X:
            setPosX((int32_t)(arg1));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_TTY_SET_POS_Y:
            setPosY((int32_t)(arg1));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_EXEC:
            qemu_log("[EXEC] File: %s | AC: %d | AS: %s",(char *)(arg1),(int32_t)(arg2),(char *)(arg3));
            //run_elf_file((char *)(arg1),0,0);
            r->eax = (uint32_t)(run_elf_file((char *)(arg1),0,0));
            break;
        case SC_CODE_TTY_PUT_COLOR:
            tty_putchar_color((char)(arg1),(uint32_t)(arg2),(uint32_t)(arg3));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_TTY_PUTS_COLOR:
            tty_puts_color((char *)(arg1),(uint32_t)(arg2),(uint32_t)(arg3));
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_TTY_DRAW_VGA:
            draw_vga_character((uint8_t)(arg1), (int32_t)(arg2), (int32_t)(arg3), (int32_t)(arg4), (int32_t)(arg5), true);
            break;
        case SC_CODE_CMOS_GET_TIME:
            r->eax = (uint32_t)get_time_pointer();
            break;
        case SC_CODE_SHUTDOWN:
            shutdown();
            r->eax = (uint32_t)1;
            break;
        case SC_CODE_REBOOT:
            reboot();
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
        case SC_CODE_malloc: {
            r->eax = (uint32_t)kheap_malloc((uint32_t)arg1);
            //tty_printf("Now memory (%d) in: %x\n", arg1, r->eax);
            break;
        }
        case SC_CODE_free: {
            kheap_free((void*)arg1);
            r->eax = (uint32_t)1;
            break;
        }
        case SC_CODE_setdev: // Хранилище
            r->eax = (uint32_t)-1;
            break;
        case SC_CODE_readfile:
            r->eax = (uint32_t)vfs_read((char *)arg1, (int32_t)arg2, (int32_t)arg3, (void *)arg4);
            //tty_printf("readfile(): got arguments: %s, %d, %d, %x\n", arg1, arg2, arg3, arg4);
            break;
        case SC_CODE_exists:
            r->eax = (uint32_t)vfs_exists((char *)arg1);
            break;
        case SC_CODE_filesize:
        	r->eax = (uint32_t)vfs_get_size((char*)arg1);
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
        case SC_CODE_ticks: 
            r->eax = timer_get_ticks();
            break;
        case SC_CODE_getfreq:
        	r->eax = timer_get_frequency();
        	break;
        case SC_CODE_getttyinfo:
        	r->eax = (uint32_t)get_tty_info();
        	break;
        case SC_CODE_get_time:
            r->eax = (uint32_t)get_time_pointer();
            break;
        case SC_CODE_punch: {
            punch();
            r->eax = 0;
            break;
        }
        default:
            qemu_log("Invalid syscall #%x", r->eax);
            qemu_log("r->idt_index = %x eax = %x  ebx = %x  "
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
