struct SynFS_file {
    uint8_t magic;
    int8_t  filename[256];
    int8_t  perms;

} SynFS_file_t __attribute__((packed));

typedef struct SynFS_file SynFS_file_t;
