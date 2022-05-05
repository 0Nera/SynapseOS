#include <kernel.h>

int SSFS_device = 0;
char SSFS_buffer[512];

void SSFS_set_device(int device){
    SSFS_device = device;
}


char *SSFS_read(char filename[12]){
    for (uint32_t i = 0; i < ide_get_size(SSFS_device); i++){
        char temp[500];
        ide_read_sectors(SSFS_device, 1, i, (uint32_t)SSFS_buffer);
        tty_printf("sector: %d[%s]\n", i, SSFS_buffer);

        if (strncmp(SSFS_buffer, filename, 12) == 0){
            strcpy(temp, &SSFS_buffer[12]);
            return temp;
        }
    }
    
}


void SSFS_format(int device){
    SSFS_set_device(device);
    memset(SSFS_buffer, 0, 512);

    for (uint32_t i = 0; i < ide_get_size(SSFS_device); i++){
        ide_write_sectors(SSFS_device, 1, i, (uint32_t)SSFS_buffer);
    }

    char SSFS_buffer[512];
    memset(SSFS_buffer, 0x90, 512);

    strcpy(SSFS_buffer, "Simple Sector FileSystem");

    ide_write_sectors(SSFS_device, 1, 1, (uint32_t)SSFS_buffer);

    memset(SSFS_buffer, 0, 512);
    strcpy(SSFS_buffer, "12345678.txt;HelloWorld!");
    ide_write_sectors(SSFS_device, 1, 2, (uint32_t)SSFS_buffer);
    char data[500];
    strcpy(data, SSFS_read("12345678.txt"));
    tty_printf("File: [%s]\n", data);
}





void SSFS_write();
void SSFS_delite();