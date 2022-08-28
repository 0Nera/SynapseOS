/*
    Взято из https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    Под лицензией MIT license
*/


#include <kernel.h>

#define MOUNTPOINTS_SIZE 100

int32_t __vfs_init = 0;
int32_t vfs_lastmnt = 0;
vfs_mount_info_t **vfs_mount_points = 0;

void vfs_mount_list() {
    for (int32_t i = 0; i < vfs_lastmnt; ++i) {
        qemu_log("%s on %s type ", vfs_mount_points[i]->fs->dev->name, vfs_mount_points[i]->location);

        if (vfs_mount_points[i]->fs->fs_type == 0) {
            qemu_log("initrd");
        } else if (vfs_mount_points[i]->fs->fs_type == 1) {
            qemu_log("ext2");
        } else if (vfs_mount_points[i]->fs->fs_type == 2) {
            qemu_log("ext3");
        } else if (vfs_mount_points[i]->fs->fs_type == 3) {
            qemu_log("ext4");
        } else if (vfs_mount_points[i]->fs->fs_type == 4) {
            qemu_log("fat32");
        } else {
            qemu_log("none");
        }
    }
}

int32_t vfs_mount(vfs_storage_dev_t *dev, vfs_filesystem_handles_t *fs_handles, int32_t type, char *location, int32_t block_size) {
    for (int32_t i = 0; i < vfs_lastmnt; ++i) {
        if (strcmp(vfs_mount_points[i]->location, location) == 0) {
            qemu_log("VFS: Device %s already mounted.", location);
            return 0;
        }
    }

    vfs_mount_info_t *mnt = (vfs_mount_info_t*) kheap_malloc(sizeof(vfs_mount_info_t));
    mnt->fs = (vfs_filesystem_t*) kheap_malloc(sizeof(vfs_filesystem_t));
    mnt->fs->dev = dev;
    mnt->fs->block_size = block_size;
    mnt->fs_handles = fs_handles;
    mnt->fs->fs_type = type;
    mnt->location = location;

    vfs_mount_points[vfs_lastmnt] = mnt;
    vfs_lastmnt++;
    qemu_log("VFS: Mounted new device at %s", location);

    return 1;
}

int32_t vfs_mount_find(char *path, int32_t *filename_add) {
    char *a = (char*) kheap_malloc(strlen(path) + 1);
    memset(a, 0, strlen(path) + 1);
    memcpy(a, path, strlen(path) + 1);
    
    int32_t at = 0;
    if (a[strlen(a)] == '/') {
        str_bksp(a, '/');
    }

    while (1) {
        for (int32_t i = 0; i < MOUNTPOINTS_SIZE; ++i) {
            if (!vfs_mount_points[i]) {
                break;
            }
            if (strcmp(vfs_mount_points[i]->location, a) == 0) {
                /* Adjust the orig to make it relative to fs/dev */
                *filename_add = strlen(a) - 1;
                kheap_free(a);
                return i;
            }
        }

        if (strcmp(a, "/") == 0) {
            break;
        }
        str_bksp(a, '/');
    }

    return 0;
}


int32_t vfs_read(const char *filename, int32_t offset, int32_t size, void *buf) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find((char*)filename, &a);
    filename += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->read == 0) {
        return 0;
    }
    vfs_mount_points[mntn]->fs_handles->read((char*)filename, offset, size, vfs_mount_points[mntn]->fs, buf);

    return 1;
}


int32_t vfs_get_size(const char *filename) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find((char*)filename, &a);
    filename += a + 1; // Change the pointer (its not const, but char its pointing to is const)

    if (vfs_mount_points[mntn]->fs_handles->read == 0) {
        return 0;
    }

    return vfs_mount_points[mntn]->fs_handles->get_size((char*)filename, vfs_mount_points[mntn]->fs);
}


int32_t vfs_is_dir(char *filename) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find(filename, &a);
    filename += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->is_dir == 0) {
        return 0;
    }
    return vfs_mount_points[mntn]->fs_handles->is_dir((char*)filename, vfs_mount_points[mntn]->fs);
}


int32_t vfs_write(char *filename, int32_t offset, int32_t size, void *buf) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find(filename, &a);
    filename += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->write == 0) {
        return 0;
    }
    vfs_mount_points[mntn]->fs_handles->write((char*)filename, offset, size, vfs_mount_points[mntn]->fs, buf);

    return 1;
}

int32_t vfs_mkdir(char *filename, char *path, uint16_t perms) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find(path, &a);
    path += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->mkdir == 0) {
        return 0;
    }
    vfs_mount_points[mntn]->fs_handles->mkdir(path, filename, vfs_mount_points[mntn]->fs, perms);

    return 1;
}

int32_t vfs_mkfile(char *filename, char *path, uint16_t perms) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find(path, &a);
    path += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->mkfile == 0) {
        return 0;
    }
    vfs_mount_points[mntn]->fs_handles->mkfile(filename, path, vfs_mount_points[mntn]->fs, perms);

    return 1;
}


int32_t vfs_list(char *path, void *buf) {
    if (strcmp(path, "/") == 0) {
        for (int32_t i = 0; i < vfs_lastmnt; ++i) {
            tty_printf(" %s ", vfs_mount_points[i]->location);
        }

        return 1;
    }

    int32_t a = 0;
    int32_t mntn = vfs_mount_find(path, &a);
    path += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->readdir == 0) {
        return 0;
    }
    vfs_mount_points[mntn]->fs_handles->readdir(path, vfs_mount_points[mntn]->fs, buf);

    return 1;
}


int32_t vfs_rm(char *filename) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find(filename, &a);
    filename += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->rm == 0) {
        return 0;
    }
    vfs_mount_points[mntn]->fs_handles->rm(filename, vfs_mount_points[mntn]->fs);

    return 1;
}

int32_t vfs_exists(const char *filename) {
    int32_t a = 0;
    int32_t mntn = vfs_mount_find((char*)filename, &a);
    filename += a + 1;

    if (vfs_mount_points[mntn]->fs_handles->read == 0) {
        return 0;
    }
    return vfs_mount_points[mntn]->fs_handles->exists((char*)filename, vfs_mount_points[mntn]->fs);
}

void vfs_get_file_name_from_path(char *fpath, char *buf) {
    int32_t len = strlen(fpath), last_slash, was_slash = 0, p1, i;
    last_slash = len - 1;

    for (i = len - 1; i >= 0; i--) {
        if (fpath[i] == '/') {
            last_slash = i;
            was_slash = 1;
            break;
        }
    }

    if (!was_slash) {
        p1 = 0;
    } else {
        if (last_slash == len - 1) {
            buf[0] = '\0';
            return;
        } else {
            p1 = last_slash + 1;
        }
    }

    for (i = p1; i < len; i++) {
        buf[i - p1] = fpath[i];
    }
    buf[len - p1] = '\0';
}

void vfs_init() {   
    vfs_mount_points = (vfs_mount_info_t**) kheap_malloc(sizeof(vfs_mount_info_t) * MOUNTPOINTS_SIZE);
    __vfs_init = 1;
    qemu_log("VFS installed");
}
