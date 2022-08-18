/**
 * @file kernel.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Входная точка ядра, инициализация драйверов
 * @version 0.1.13
 * @date 2022-08-13
 * @copyright Copyright Арен Елчинян (c) 2022
 */

/*
    Ядро SynapseOS

    
*/

#include <kernel.h>
#include <drivers/ata.h>


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

    // Вывод информации о ядре
    tty_printf("\t\tSynapseOS kernel version: %d.%d.%d, Built: %s\n", 
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

    vfs_init();                             // Инициализация виртуальной файловой системы

    initrd_init(initrd_beg, initrd_end);    // Инициализация ramdisk

    syscall_init();                         // Инициализация системного api для программ

    keyboard_install();                     // Установка драйвера клавиатуры

    timer_install();                        // Установка PIT

    pci_init();                             // Установка драйвера PCI

    struct synapse_time TIME = get_time();
    tty_printf("Current datetime is: %d/%d/%d %d:%d:%d (unix:%d)\n", TIME.day, TIME.month,
    							TIME.year, TIME.hours, TIME.minutes, TIME.seconds,
    							synapse_time_to_unix(TIME));
    tty_puts("Experimental font demo: \xFF\x01 \xFF\x02 \xFF\x03 \xFF\x04 \xFF\x05 \xFF\x06 \xFF\x07 \xFF\x08 \xFF\x09 \xFF\x0A \xFF\x0B \xFF\x0C\n");
    tty_puts("Colors: \xFF\x0D\xFF\x0E\xFF\x0F\n");
    tty_printf("Test: -1=[%d]  \n", -1);

    // tty_printf("%s", "\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\xFF\x0C\n");
    // ^--- Без %s не работает. Только так или используйте tty_puts()

    //ata_init();                             // Установка драйвера ATA

    unit_test(RTL8139_init());              // Тестируем драйвер RTL8139

    vfs_mount_list();                       // Выводим список корня VFS
    detect_cpu(1);
    // while(1){
    //     asm volatile("hlt");
    // }
    //qemu_log("task: %d", create_task());
    //init_task_manager();
    //create_STFS(0);

    if (floppy_install()){
        
    } else {
        tty_printf("\nFloppy: ERROR\n");
    }
    //tty_printf("\n%d\n%s\n", 101/10, format_string("Hello! %d, %x, %c, %s", 120, 0x90, 'H', "Hi!"));
    
    //duke_draw_scaled("/initrd/res/Sovrum/Carol_Tea_Theme.duke", getWidthScreen()-1, getHeightScreen()-1, 0, 0);
    duke_draw_from_file("/initrd/res/SynapseOSLogo.duke", getWidthScreen()-106, 36);

    shell();                                // Активация терминала
    // Работает, но надо доделать 
    //run_driver_thread("/initrd/sys/shell.sea");
    //run_driver_thread("/initrd/sys/user.sea");
    //run_driver_thread("/initrd/sys/interface.sea");
    //run_driver_thread("/initrd/sys/kernel.elf");
}

