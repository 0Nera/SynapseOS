#include <kernel.h>


void create_STFS(int device) {
    tty_printf("\nThis action will remove all data in device %d, enter y(es)/n(not):\n", device);
    char buffer[512];
    //char cmd_char = keyboard_getchar();
    int sectors_max = 0;

    //if (cmd_char != 'y'){
    //    return;
    //}

    
    sectors_max = ide_get_size(device);

    // Запись загрузчика
    memset(buffer, 0, 512);
    ide_write_sectors(device, 1, 0, (uint32_t)buffer);
    
    // Очистка диска
    
    for (int i = 2; i < sectors_max; i++){
        ide_write_sectors(device, 1, i, (uint32_t)buffer);
    }

    // Запись метаданных ФС
    buffer[0] = 0x90;
    buffer[1] = 'S';
    buffer[2] = 'T';
    buffer[3] = 'F';
    buffer[4] = 'S';
    buffer[5] = 0;
    buffer[6] = 1;
    buffer[7] = 0;

    ide_write_sectors(device, 1, 1, (uint32_t)buffer);
}


void SSFS_create_folder_raw(uint8_t folder_name[256], int block){
/*
    char block_part_1[512];
    char block_part_2[512];
    char block_part_3[512];
    char block_part_4[512];
*/
}


void SSFS_create_file_raw(uint8_t filename[256]){

}
