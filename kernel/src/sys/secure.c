#include <kernel.h>


/*
    0 - защиты нет
    1 - слабая защита
*/
static uint8_t level = 1;


void secure_init(uint8_t new_level) {
    level = new_level;
    qemu_log("Secure level: %d", level);
}


bool secure_check_elf(const char *name) {
    if (!vfs_exists(name)) {
        return false;
    }

    void *elf_file = elf_open(name);
    
    if (elf_file == NULL) {
        return false;
    }

    int i = 256;

    while(i--){
        
        //tty_printf("%d;", elf_file[i]);
        
    }

    tty_printf("\n");

    return true;
}
