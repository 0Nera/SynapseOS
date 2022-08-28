/**
 * @file kernel.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Входная точка ядра, инициализация драйверов
 * @version 0.1.13
 * @date 2022-08-13
 * @copyright Copyright Арен Елчинян (c) 2022
 */


#include <kernel.h>
#include <drivers/ata.h>


int32_t errno = 0;
uint32_t os_mode = 1; // 0 - мало ОЗУ, 1 - обычный режим, 2 - режим повышенной производительности, 3 - сервер


/*!
	\brief Входная точка ядра SynapseOS
	\warning Отсутствует проверка multiboot!
*/
void kernel(uint32_t magic_number, struct multiboot_info *mboot_info) {
    if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC) {
        qemu_log("Invalid magic number: %x, valid = %x", magic_number, MULTIBOOT_BOOTLOADER_MAGIC);
    }

    tty_init(mboot_info);   // Настройка графики

    gdt_init(); // \    Установка и настройка прерываний 
    idt_init(); // /
    pmm_init(mboot_info);

    uint32_t initrd_beg = *(uint32_t*) (mboot_info->mods_addr);     // Адрес начала ramdisk
    uint32_t initrd_end = *(uint32_t*) (mboot_info->mods_addr + 4); // Адрес конца ramdisk
    qemu_log("initrd_beg = %x initrd_end = %x",
        initrd_beg, initrd_end              // Вывод информации о адресах ramdisk в отладчик
    );

    vmm_init();                             // Инициализация менеджера виртуальной памяти

    kheap_init();                           // Инициализация кучи ядра

    init_vbe(mboot_info);                   // Активация графики 1024x768

    // Вывод информации о ядре
    tty_printf("\nSynapseOS kernel version: %d.%d.%d, Built: %s\n", 
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,    // Версия ядра 
        __TIMESTAMP__                                   // Время окончания компиляции ядра
        );

    // Проверка, является ли сборка дистрибутивом
    if (isDistr) {
        //Вывод информации о дистрибутиве
        tty_printf("\tDistribution \"%s\", version %d.%d.%d\n",
            DistrName, dVERSION_MAJOR, dVERSION_MINOR, dVERSION_PATCH
            );
    }

    vfs_init();                             // Инициализация виртуальной файловой системы

    initrd_init(initrd_beg, initrd_end);    // Инициализация ramdisk

    syscall_init();                         // Инициализация системного api для программ

    keyboard_install();                     // Установка драйвера клавиатуры

    timer_install();                        // Установка PIT

    pci_init();                             // Установка драйвера PCI

    struct synapse_time TIME = get_time();
    tty_printf("Current datetime is: %d/%d/%d %d:%d:%d\n", TIME.day, TIME.month,
    							TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    tty_puts("Experimental font demo: \xFF\x01 \xFF\x02 \xFF\x03 \xFF\x04 \xFF\x05 \xFF\x06 \xFF\x07 \xFF\x08 \xFF\x09 \xFF\x0A \xFF\x0B \xFF\x0C\n");
    tty_puts("Colors: \xFF\x0D\xFF\x0E\xFF\x0F\n");
    tty_puts_color("Circles: [\xFF\x10][\xFF\x11]\n", 0, 0xFF0000);
    tty_printf("Test: -1=[%d]  \n", -1);

    unit_test(RTL8139_init());              // Тестируем драйвер RTL8139

    vfs_mount_list();                       // Выводим список корня VFS
    detect_cpu(1);

    //ATA_Drive_t *drive;
    //ATA_Detect(&drive);

    sb16_init();

    if(vfs_exists("/initrd/res/sound.wav")) {
        tty_printf("EXISTS!\n");

        int fsize = vfs_get_size("/initrd/res/sound.wav");
        char* fdat = kheap_malloc(fsize);
        memset(fdat, 0, fsize);
        tty_printf("FILE SIZE: %d\n", fsize);

        // vfs_read("/initrd/res/sound.wav", 0, fsize, fdat);

        sb16_play_audio(fdat, 44100, 1, 0, 1, fsize);

        kheap_free(fdat);
    }

    duke_draw_from_file("/initrd/res/SynapseOSLogo.duke", getWidthScreen()-100, 40);
    
    shell();                                // Активация терминала
    // Работает, но надо доделать 
    //run_driver_thread("/initrd/sys/shell.sea");
    //run_driver_thread("/initrd/sys/user.sea");
    //run_driver_thread("/initrd/sys/interface.sea");
    //run_driver_thread("/initrd/sys/kernel.elf");
}

