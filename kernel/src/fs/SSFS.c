#include <kernel.h>


int SSFS_device = 0;
char SSFS_buffer[500];


void SSFS_set_device(int device){
    SSFS_device = device;
}


int SSFS_file_exists(char filename[12]){
    for (uint32_t i = 2; i < ide_get_size(SSFS_device); i++){
        char fname[13];
        char buffer[512];

        ide_read_sectors(SSFS_device, 1, i, (uint32_t)buffer);

        for (int j = 0; j < 12; j++){
            fname[j] = buffer[j];
        }

        fname[13] = 0;

        if (strcmp(fname, filename) == 0){
            return i;
        }
    }
    return 0;
}


int SSFS_write(char filename[12], char buffer[500]){
    int file_id = SSFS_file_exists(filename);

    if (file_id){
        char fname[13];

        ide_read_sectors(SSFS_device, 1, file_id, (uint32_t)SSFS_buffer);

        for (int j = 0; j < 12; j++){
            fname[j] = SSFS_buffer[j];
        }

        fname[13] = 0;
            
        return 1;
    }
    return 0;
    
}


int SSFS_read(char filename[12], char buffer[500]){
    int file_id = SSFS_file_exists(filename);

    if (file_id){
        memset(SSFS_buffer, 0, 500);

        ide_read_sectors(SSFS_device, 1, file_id, (uint32_t)SSFS_buffer);

        strcpy(buffer, &SSFS_buffer[12]);
            
        return 1;
    }
    return 0;
    
}



void SSFS_format(int device){
    SSFS_set_device(device);
    memset(SSFS_buffer, 0, 512);

    for (uint32_t i = 0; i < ide_get_size(SSFS_device); i++){
        ide_write_sectors(SSFS_device, 1, i, (uint32_t)SSFS_buffer);
    }

    memset(SSFS_buffer, 0, 512);

    strcpy(SSFS_buffer, "Simple Sector FileSystem");

    ide_write_sectors(SSFS_device, 1, 1, (uint32_t)SSFS_buffer);

    memset(SSFS_buffer, 0, 512);
    strcpy(SSFS_buffer, "Hi World.txtHelloWorld!");
    ide_write_sectors(SSFS_device, 1, 2, (uint32_t)SSFS_buffer);

    char data[500];
    memset(&data, 0, 500);
    SSFS_read("Hi World.txt", data);
    tty_printf("data: [%s] %d %d\n", data, strlen(data), data);
}





void SSFS_delite();