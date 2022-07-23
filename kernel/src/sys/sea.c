/*
    Загрузчик SEA файлов
*/
#include <kernel.h>


int32_t sea_validate(char *data){
    if (data[0] != 0x60 || 
        data[1] != 0x05 || 
        data[2] != 0x05 || 
        data[3] != 'S' || 
        data[4] != 'Y' || 
        data[5] != 'N' || 
        data[6] != '8' || 
        data[7] != '6' 
        ) {
        return 1; 
    } 
    return 0;
}


void *sea_load(const char *fname){
    if (!vfs_exists(fname)) {
        tty_printf("app doesnt exist\n");
        return 0;
    }

    uint32_t fsize = vfs_get_size(fname);

    void *addr = kheap_malloc(fsize);

    int32_t res = vfs_read(fname, 0, fsize, addr);

    return (void*)res;
}

int32_t sea_run(const char *name/*, char **argv, char **env __attribute__((unused)), int32_t argc*/) {
    void *sea_file = sea_load(name);
    if (sea_file == NULL) {
        return -1;
    }


    

    void(*entry_point)() = (void*) (sea_file);
    tty_printf("SEA entry point: %x\n", sea_file);

    entry_point();

    return 0;
}
