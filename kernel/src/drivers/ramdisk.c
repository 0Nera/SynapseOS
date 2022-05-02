#include <kernel.h>

// 

uint32_t initrd_begin = 0;
uint32_t initrd_end = 0;
uint32_t initrd_size;

int oct2bin(unsigned char *str, int size) {
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

unsigned int tar_getsize(const char *in) {
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;

    for (j = 11; j > 0; j--, count *= 8) {
        size += ((in[j - 1] - '0') * count);
    }

    return size;
}

/* Returns pointer to file data */
int tar_lookup(unsigned char *archive, char *filename) {
    unsigned char *ptr = archive;
 
    while (!memcmp(ptr + 257, "ustar", 5)) 
    {
        int filesize = oct2bin(ptr + 0x7c, 11);
        if (!memcmp(ptr, filename, strlen(filename) + 1)) {
            return ptr + 512;
        }

        ptr += (((filesize + 511) / 512) + 1) * 512;
        if (ptr == initrd_end) {
            return 0;
        }
        if (ptr >  initrd_end) {
            return 0;
        }
    }
    return 0;
}

uint32_t initrd_read(char *filename, int offset, int size, vfs_filesystem_t *fs, void *buffer) {
    int read_size = 0;
    if (!filename) {
        return 0;
    }
    if (!size) {
        return 0;
    }

    int file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // File not found
        return 0;
    }

    file_addr -= 512;
    ustar_file_t *file = (struct ustar_file_t*) file_addr;
    file_addr += 512;

    if (size > oct2bin(file->size, 11)) {
        read_size = oct2bin(file->size, 11);
    } else {
        read_size = size;
    }

    memcpy(buffer, file_addr + offset, size);

    return read_size;
}

uint32_t initrd_file_exists(char *filename, vfs_filesystem_t *fs) {
    if (!filename) {
        return 0;
    }

    int file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // file not found
        return 0;
    } else {
        return file_addr;
    }
}

uint32_t initrd_get_filesize(char *filename) {
    if (!filename) {
        return 0;
    }

    int file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // file not found
        return 0;
    } else {
        file_addr -= 512;
        ustar_file_t *file = (struct ustar_file_t*) file_addr;
        return oct2bin(file->size, 11);
    }
}

uint32_t initrd_is_dir(char *filename) {
    if (!filename) {
        return 0;
    }

    int file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // file not found
        return 0;
    } else {
        ustar_file_t *file = (struct ustar_file_t*) file_addr;
        return file->type; //(file->type == USTAR_DIRECTORY); // TODO why this comparison doenst work?? why for files file->type is 0 and for dirs id 48 aka '0' ????
    }
}

void initrd_list(int argc, char **arg) {
    int addr = initrd_begin;

    while (!memcmp(addr + 257, "ustar", 5)) {
        int filesize = oct2bin(addr + 0x7c, 11);
        ustar_file_t *file = (struct ustar_file_t*) addr;

        if (file->type == USTAR_DIRECTORY) {
            tty_printf("\n    <dir>       /initrd/%s", file->fname);
        } else if (file->type == USTAR_NORMAL_FILE) {
            tty_printf("\n    <file> %d   /initrd/%s ", filesize, file->fname);
        }
        addr += (((filesize + 511) / 512) + 1) * 512;
        
        if (addr == initrd_end) {
            break;
        }
        if (addr > initrd_end ) {
            break;
        }
    }
    tty_printf("\n");
}

void initrd_init(uint32_t phys_begin, uint32_t phys_end) {
    // TODO: map initrd image to virtual memory

    qemu_printf("\ninitrd phys begin = %x\ninitrd phys end = %x\n", phys_begin, phys_end);

    //int i;
    //uint32_t v1 = vmm_temp_map_page(phys_begin);
    //for (i = 0; i < 20; i++) tty_printf("%c", *(char*) (phys_begin + i));

    initrd_size = phys_end - phys_begin;
    initrd_begin = kheap_malloc(initrd_size + 4 * PAGE_SIZE);

    physical_addres frame;
    virtual_addr virt;
    for (frame = PAGE_ALIGN_DOWN(phys_begin), virt = PAGE_ALIGN_DOWN(initrd_begin) + PAGE_SIZE;
         frame <= (PAGE_ALIGN_DOWN(phys_end));
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        vmm_map_page(frame, virt);
    }
    initrd_begin = PAGE_ALIGN_DOWN(initrd_begin) + PAGE_SIZE + phys_begin % PAGE_SIZE;
    initrd_end = initrd_begin + initrd_size;

    //int i;
    //for (i = 0; i < 100; i++) tty_printf("%c", *(char*) (initrd_begin + i));

    //uint32_t v1 = vmm_temp_map_page(phys_begin);
    //for (i = 0; i < 4096; i++) tty_printf("%c", *(char*) (v1 + i));

    vfs_storage_dev_t *dev = kheap_malloc(sizeof(vfs_storage_dev_t));
    dev->type = 4;
    strcpy(dev->name, "initrd");

    vfs_filesystem_handles_t *fs_handles = kheap_malloc(sizeof(vfs_filesystem_handles_t));

    fs_handles->read = &initrd_read;
    fs_handles->exists = &initrd_file_exists;
    fs_handles->get_size = &initrd_get_filesize;
    fs_handles->is_dir = (uint32_t*)&initrd_is_dir;
    fs_handles->write = 0;
    fs_handles->readdir = 0;
    fs_handles->mkfile = 0;
    fs_handles->mkdir = 0;
    fs_handles->rm = 0;

    vfs_mount(dev, fs_handles, 0, "/initrd/", 0);
}
