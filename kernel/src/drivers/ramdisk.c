#include <kernel.h>

// 

uint32_t initrd_begin = 0;
uint32_t initrd_end = 0;
uint32_t initrd_size;

int32_t oct2bin(unsigned char *str, int32_t size) {
    int32_t n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

uint32_t tar_getsize(const char *in) {
    uint32_t size = 0;
    uint32_t j;
    uint32_t count = 1;

    for (j = 11; j > 0; j--, count *= 8) {
        size += ((in[j - 1] - '0') * count);
    }

    return size;
}

/* Returns pointer to file data */
int32_t tar_lookup(unsigned char *archive, char *filename) {
    unsigned char *ptr = archive;
 
    while (!memcmp(ptr + 257, "ustar", 5)) 
    {
        int32_t filesize = oct2bin(ptr + 0x7c, 11);
        if (!memcmp(ptr, filename, strlen(filename) + 1)) {
            return (int32_t)(ptr + 512);
        }

        ptr += (((filesize + 511) / 512) + 1) * 512;
        if ((uint32_t)ptr == initrd_end) {
            return 0;
        }
        if ((uint32_t)ptr >  initrd_end) {
            return 0;
        }
    }
    return 0;
}

uint32_t initrd_read(char *filename, int32_t offset, int32_t size, vfs_filesystem_t *fs, void *buffer) {
    int32_t read_size = 0;
    if (!filename) return 0;
    if (!size) return 0;

    int32_t file_addr = tar_lookup((uint8_t*)initrd_begin, filename);
    if (!file_addr) { // File not found
        return 0;
    }

    file_addr -= 512;
    ustar_file_t *file = (ustar_file_t*) file_addr;
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

    int32_t file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // file not found
        return 0;
    } else {
        return file_addr;
    }
}

uint32_t initrd_get_filesize(char *filename) {
    if (!filename) return 0;

    int32_t file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // file not found
        return 0;
    } else {
        file_addr -= 512;
        ustar_file_t *file = (ustar_file_t*) file_addr;
        return oct2bin(file->size, 11);
    }
}

uint32_t initrd_is_dir(char *filename) {
    if (!filename) return 0;

    int32_t file_addr = tar_lookup(initrd_begin, filename);
    if (!file_addr) { // file not found
        return 0;
    } else {
        ustar_file_t *file = (ustar_file_t*) file_addr;
        return file->type; //(file->type == USTAR_DIRECTORY); // TODO why this comparison doenst work?? why for files file->type is 0 and for dirs id 48 aka '0' ????
    }
}

void initrd_list(int32_t argc, char **arg) {
    int32_t addr = initrd_begin;

    while (!memcmp(addr + 257, "ustar", 5)) {
        int32_t filesize = oct2bin((uint8_t*)(addr + 0x7c), 11);
        ustar_file_t *file = (ustar_file_t*) addr;

        if (file->type == USTAR_DIRECTORY) {
            tty_printf("\n    <dir>       /initrd/%s", file->fname);
        } else if (file->type == USTAR_NORMAL_FILE) {
            tty_printf("\n     <file> %d   /initrd/%s ", filesize, file->fname);
        }
        addr += (((filesize + 511) / 512) + 1) * 512;
        
        /*if (addr == initrd_end) {
            break;
        }
        if (addr > initrd_end ) {
            break;
        }*/ // Warning: These if blocks can be collapsed into -----|
        if(addr>=initrd_end) break; //  <--------------------------|
    }
    tty_printf("\n");
}

void initrd_init(uint32_t phys_begin, uint32_t phys_end) {
    // TODO: map initrd image to virtual memory

    qemu_log("initrd phys begin = %x initrd phys end = %x", phys_begin, phys_end);


    initrd_size = phys_end - phys_begin;
    initrd_begin = (uint32_t)kheap_malloc(initrd_size + 4 * PAGE_SIZE);

    physical_addres_t frame;
    virtual_addr_t virt;
    for (frame = PAGE_ALIGN_DOWN(phys_begin), virt = PAGE_ALIGN_DOWN(initrd_begin) + PAGE_SIZE;
         frame <= (PAGE_ALIGN_DOWN(phys_end));
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        vmm_map_page(frame, virt);
    }
    initrd_begin = PAGE_ALIGN_DOWN(initrd_begin) + PAGE_SIZE + phys_begin % PAGE_SIZE;
    initrd_end = initrd_begin + initrd_size;

 
    vfs_storage_dev_t *dev = kheap_malloc(sizeof(vfs_storage_dev_t));
    dev->type = 4;
    strcpy(dev->name, "initrd");

    vfs_filesystem_handles_t *fs_handles = kheap_malloc(sizeof(vfs_filesystem_handles_t));

    fs_handles->read = &initrd_read;
    fs_handles->exists = &initrd_file_exists;
    fs_handles->get_size = &initrd_get_filesize;
    fs_handles->is_dir = &initrd_is_dir;
    fs_handles->write = 0;
    fs_handles->readdir = 0;
    fs_handles->mkfile = 0;
    fs_handles->mkdir = 0;
    fs_handles->rm = 0;

    vfs_mount(dev, fs_handles, 0, "/initrd/", 0);
}
