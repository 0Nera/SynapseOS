#include <kernel.h>


int SSFS_device = 0;        // Текущее устройство
int SSFS_MAX = 4096;  // Максимум файлов
char SSFS_filename[12];      // Буффер содержимого
char SSFS_data[500];      // Буффер содержимого


void SSFS_set_device(int device){
    SSFS_device = device;
}


int SSFS_file_exists(char filename[12]){
    for (uint32_t i = 2; i < SSFS_MAX; i++){
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


int SSFS_find_free(){
    for (uint32_t i = 2; i < SSFS_MAX; i++){
        char buffer[512];

        ide_read_sectors(SSFS_device, 1, i, (uint32_t)buffer);

        if (buffer[0] == 0) {
            return i;
        }
    }
    return -1;
}


int SSFS_list(){
    char fname[13];
    char buffer[512];
    int len = 0;

    tty_printf("\nList of files:\n");

    for (uint32_t i = 2; i < SSFS_MAX; i++){
        ide_read_sectors(SSFS_device, 1, i, (uint32_t)buffer);

        if (buffer[0] == 0) {
            continue;
        }

        for (int j = 0; j < 12; j++){
            fname[j] = buffer[j];
        }

        fname[13] = 0;
        len++;
        tty_printf("%d->%s\n", len, fname);
    }
    return len;
}


int SSFS_write(char filename[12], char buffer[500]){
    char sector[512];

    if (!SSFS_file_exists(filename)){
        memset(sector, 0, 512);
        strcpy(sector, filename);
        strcpy(&sector[12], buffer);
        ide_write_sectors(SSFS_device, 1, SSFS_find_free(), (uint32_t)sector);
        return 1;
    }
    return -1;
}


int SSFS_read(char filename[12], char buffer[500]){
    int file_id = SSFS_file_exists(filename);

    if (file_id){
        memset(SSFS_data, 0, 500);
        ide_read_sectors(SSFS_device, 1, file_id, (uint32_t)SSFS_data);
        strcpy(buffer, &SSFS_data[12]);
        return 1;
    }
    return 0;
}


void SSFS_delete(char filename[12]){
    char sector[512];
    int file_id = SSFS_file_exists(filename);

    if (file_id){
        memset(sector, 0, 512);
        ide_write_sectors(SSFS_device, 1, file_id, (uint32_t)sector);
        return 1;
    }
    return -1;
}


void SSFS_format(int device){
    tty_printf("\nThis action will remove all data in device %d, enter y(es)/n(not):\n", SSFS_device);
    char buffer[512];
    char SSFS_cmd_char = keyboard_getchar();

    if (SSFS_cmd_char != 'y'){
        return;
    }

    strcpy(SSFS_data, "SSFS");

    if (ide_get_size(device) < 4096) {
        SSFS_MAX = ide_get_size(device);
    }

    SSFS_data[4] = 1; // Версия
    SSFS_data[5] = 2; // Начало
    SSFS_data[6] = SSFS_MAX; // Конец

    tty_printf("\nFormating device: %d\n", SSFS_device);

    SSFS_set_device(device);
    memset(buffer, 0, 512);

    for (uint32_t i = 0; i < SSFS_data[6]; i++){
        ide_write_sectors(SSFS_device, 1, i, (uint32_t)buffer);
    }

    ide_write_sectors(SSFS_device, 1, 1, (uint32_t)SSFS_data);

    memset(SSFS_data, 0, 500);
    strcpy(SSFS_data, "Hello World!\nI'm the Single Sector FileSystem!\n Now I can:\n->Read and write single-sector files\n->Format hard drives\n->Search for empty and busy sectors");
    SSFS_write("test.txt", SSFS_data);

    memset(&SSFS_data, 0, 500);
    SSFS_read("test.txt", SSFS_data);
}


void SSFS_cmd() {
    int cmd_alive = 1;

    while(cmd_alive){
        char *filename;
        char *buffer;
        int n;

        tty_printf("\nSingle Sector FileSystem:\n" \ 
                "->1 Set drive\n"             \ 
                "->2 Format drive\n"          \ 
                "->3 list of files\n"          \ 
                "->4 Read file\n"             \ 
                "->5 Write file\n"            \ 
                "->6 Delete file\n"           \
                "->9 Exit\n"                  \
                "Enter only number:"
            );

        char *SSFS_cmd = keyboard_gets();

        if (strlen(SSFS_cmd) == 0) {
            continue;
        } else if (strcmp(SSFS_cmd, "1") == 0) {
            tty_printf("\nEnter driver ID <0-4>:\n");
            buffer = keyboard_gets();

            if (strcmp(buffer, "0") == 0) {
                SSFS_set_device(0);
            } else if (strcmp(buffer, "1") == 0) {
                SSFS_set_device(1);
            } else if (strcmp(buffer, "3") == 0) {
                SSFS_set_device(2);
            } else if (strcmp(buffer, "4") == 0) {
                SSFS_set_device(4);
            }
        } else if (strcmp(SSFS_cmd, "2") == 0) {
            SSFS_format(0);
        } else if (strcmp(SSFS_cmd, "3") == 0) {
            SSFS_list();
        } else if (strcmp(SSFS_cmd, "4") == 0) {
            tty_printf("\nEnter filename:\n");
            filename = keyboard_gets();
            SSFS_read(filename, buffer);
            tty_printf("\nData: [%s]\n", buffer);
        } else if (strcmp(SSFS_cmd, "5") == 0) {
            tty_printf("\nEnter filename:\n");
            filename = keyboard_gets();
            tty_printf("\nEnter data:\n");
            buffer = keyboard_gets();
            SSFS_write(filename, buffer);
        } else if (strcmp(SSFS_cmd, "6") == 0) {
            tty_printf("\nEnter filename:\n");
            filename = keyboard_gets();
            SSFS_delete(filename);
        } else if (strcmp(SSFS_cmd, "9") == 0) {
            cmd_alive = 0;
        } else{
        }
    }
}