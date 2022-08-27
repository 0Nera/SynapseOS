#pragma once
#include <stdbool.h>
#include <stdint.h>


/*
    0 - диск не используется, 
    1 - AHCI, 
    2 - ATA, 
    3 - Устройство хранения, 
    4 - Другое
*/
typedef struct vfs_storage_dev_t {
    int32_t type; 

    int32_t disk_id;
    int32_t partition_id;

    int32_t lba_start;
    int32_t lba_end;

    char *name;

    uint32_t (*read) (int*, unsigned long long, unsigned long long, uint16_t*);
    uint32_t (*write) (int*, unsigned long long, unsigned long long, uint16_t*);
} vfs_storage_dev_t;

// Некоторые файловые системы
typedef struct vfs_filesystem_t {
    int32_t fs_type; // 0 - tar-fs, 1 - ext2, 2 - ext3, 3 - ext4, 4 - fat32
    int32_t block_size;

    vfs_storage_dev_t *dev;
} vfs_filesystem_t;

// Содержит указатели на некоторые методы файловой системы.
typedef struct vfs_filesystem_handles_t {
    uint32_t (*read) (char *fn, int, int, vfs_filesystem_t*, void*);
    uint32_t (*write) (char *fn, int, int, vfs_filesystem_t*, void*);

    uint32_t (*readdir) (char*, vfs_filesystem_t*, void*);

    uint32_t (*mkfile) (char *fn, char*, vfs_filesystem_t*, uint16_t);
    uint32_t (*mkdir) (char *fn, char*, vfs_filesystem_t*, uint16_t);

    uint32_t (*rm) (char *fn, vfs_filesystem_t*);
    uint32_t (*ls) (char *fn, vfs_filesystem_t*);

    uint32_t (*get_size) (char *fn, vfs_filesystem_t*);
    uint32_t (*exists) (char *fn, vfs_filesystem_t*);
    uint32_t (*is_dir) (char *fn, vfs_filesystem_t*);
} vfs_filesystem_handles_t;

// Точка монтирования VFS
typedef struct vfs_mount_info_t  {
    char *location;
    vfs_filesystem_t *fs;
    vfs_filesystem_handles_t *fs_handles;
} vfs_mount_info_t;

void vfs_init();

void vfs_mount_list();
int32_t vfs_mount(vfs_storage_dev_t *dev, vfs_filesystem_handles_t *fs_handles, int32_t type, char *location, int32_t block_size);
int32_t vfs_mount_find(char *path, int32_t *filename_add);

int32_t vfs_read(const char *filename, int32_t offset, int32_t size, void *buf);
int32_t vfs_get_size(const char *filename);
int32_t vfs_write(char *filename, int32_t offset, int32_t size, void *buf);

int32_t vfs_mkdir(char *filename, char *path, uint16_t perms);
int32_t vfs_mkfile(char *filename, char *path, uint16_t perms);

int32_t vfs_list(char *path, void *buf);
int32_t vfs_rm(char *filename);

int32_t vfs_exists(const char *filename);
int32_t vfs_is_dir(char *filename);

void vfs_get_file_name_from_path(char *fpath, char *buf); 
