#pragma once


#include <drivers/vfs.h>
#include <stdbool.h>
#include <stdint.h>

/*
    tar-fs из EOS 
*/
#define USTAR_NORMAL_FILE      '0'
#define USTAR_HARD_LINK        '1'
#define USTAR_SYMBOLIC_LINK    '2'
#define USTAR_CHARACTER_DEVICE '3'
#define USTAR_BLOCK_DEVICE     '4'
#define USTAR_DIRECTORY        '5'
#define USTAR_NAMED_PIPE       '6'

/**
 * @brief Структура файла
 * 
 */
typedef struct {
    char fname[100];
    char fmode[8];

    char uid[8];
    char gid[8];

    char size[12];
    char mtime[12];

    char checksum[8];
    char type;

    char linkname[100];
    char signature[6];
    char version[2];

    char user_name[32];
    char group_name[32];

    char device_major_number[8];
    char device_minor_number[8];

    char fprefix[155];

    char reserved[12];
} __attribute__((packed)) ustar_file_t;

int32_t oct2bin(unsigned char *str, int32_t size);

uint32_t tar_getsize(const char *in);
int32_t tar_lookup(unsigned char *archive, char *filename);

uint32_t initrd_read(char *filename, int32_t offset, int32_t size, vfs_filesystem_t *u, void *buffer);

uint32_t initrd_file_exists(char *filename, vfs_filesystem_t *u);
uint32_t initrd_get_filesize(char *filename);

uint32_t initrd_is_dir(char *filename);

void initrd_list(int32_t argc, char **arg);
void initrd_init(uint32_t begin, uint32_t end);