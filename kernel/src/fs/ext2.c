#include <stdint.h>

#define EXT2_DIRECT_BLOCKS 12


#define SUPERBLOCK_SIZE 1024
#define ROOT_INODE_NUMBER 2

#define EXT2_S_IFSOCK   0xC000
#define EXT2_S_IFLNK    0xA000
#define EXT2_S_IFREG    0x8000
#define EXT2_S_IFBLK    0x6000
#define EXT2_S_IFDIR    0x4000
#define EXT2_S_IFCHR    0x2000
#define EXT2_S_IFIFO    0x1000

struct ext2_super_block {
    uint32_t   s_inodes_count;        //  Количество индексных дескрипторов в ФС 
    uint32_t   s_blocks_count;        //  Количество блоков в ФС 
    uint32_t   s_r_blocks_count;      //  Количество зарезервированых блоков 
    uint32_t   s_free_blocks_count;   //  Количество свободных блоков 
    uint32_t   s_free_inodes_count;   //  Количество свободных индексных дескрипторов 
    uint32_t   s_first_data_block;    //  Номер данного блока 
    uint32_t   s_log_block_size;      //  log2(размер блока)-10 
    int32_t    s_log_frag_size;       //  log2(размер фрагмента)-10 
    uint32_t   s_blocks_per_group;    //  #  Кол-во блоков в группе 
    uint32_t   s_frags_per_group;     //  #  Кол-во фрагментов в группе 
    uint32_t   s_inodes_per_group;    //  #  Кол-во индексных дескрипторов в группе 
    uint32_t   s_mtime;               //  Время последнего монтирования в POSIX времени 
    uint32_t   s_wtime;               //  Время последней записи в POSIX времени 
    uint16_t   s_mnt_count;           
    int16_t    s_max_mnt_count;       
    uint16_t   s_errors;              //  Код ошибки(см.ниже) 
    uint16_t   s_pad;
    uint32_t   s_lastcheck;           //  POSIX время последней проверки 
    uint32_t   s_checkinterval;       //  POSIX время между принудительными проверками 
    uint32_t   s_creator_os;          //  ID ОС (см. ниже)  
    uint32_t   s_rev_level;           //  Версия 
    
    
    uint16_t   s_def_resuid;          //  UserID, могущий использовать зар. блоки 
    uint16_t   s_def_resgid;          //  GroupID, могущий использовать зар. блоки 
    uint32_t   s_reserved[235];       //  Зарезервировано 
};