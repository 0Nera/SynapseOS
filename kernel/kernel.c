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
bool autotshell = false;
bool rtl8139_load = true;

void kernelCMDHandler(char* cmd){
    qemu_log("[kCMD] '%s'",cmd);
    if (strcmpn(cmd,"--bootscreen=minimal")){
        bootScreenChangeMode(1);
        qemu_log("[kCMD] The minimum operating mode for BootScreen is selected.");
    }
    if (strcmpn(cmd,"--tshell")){
        autotshell = true;
        qemu_log("[kCMD] After loading the kernel, TShell will automatically start.");
    }
    if (strcmpn(cmd,"--nortl8139")){
        rtl8139_load = false;
        qemu_log("[kCMD] The Realtek RTL8139 driver will not be loaded on kernel startup.");
    }

    //split(cmd, " ");
    //debugSplit();
    /**
    qemu_log("[kCMD] tshell: %d",(isKeySplit("tshell") == true?1:0));
    if (isKeySplit("--bootscreen=minimal")){
        //qemu_log("[kCMD] The minimum operating mode for BootScreen is selected.");
        bootScreenChangeMode(1);
    }
    if (isKeySplit("--tshell")){
        //qemu_log("[kCMD] After loading the kernel, TShell will automatically start.");
        autotshell = true;
    }
    if (isKeySplit("--no-rtl8139")){
        //qemu_log("[kCMD] The Realtek RTL8139 driver will not be loaded on kernel startup.");
        rtl8139_load = false;
    }
    */
}


/*!
	\brief Входная точка ядра SynapseOS
	\warning Отсутствует проверка multiboot!
*/
void kernel(uint32_t magic_number, struct multiboot_info *mboot_info) {
    if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC) {
        qemu_log("Invalid magic number: %x, valid = %x", magic_number, MULTIBOOT_BOOTLOADER_MAGIC);
    }
    tty_init(mboot_info);   // Настройка графики

    char* kCMD = mboot_info->cmdline;
    kernelCMDHandler(kCMD);
    // Загружаем bootScreen
    bootScreenInit(10);
    bootScreenLazy(true);

    bootScreenPaint("Setting `Global Descriptor Table`...");
    gdt_init(); // Установка и настройка прерываний

    bootScreenPaint("Setting `Interrupt Descriptor Table`...");
    idt_init(); //

    bootScreenPaint("Configuring the Physical Storage Manager...");
    pmm_init(mboot_info);



    uint32_t initrd_beg = *(uint32_t*) (mboot_info->mods_addr);     // Адрес начала ramdisk
    uint32_t initrd_end = *(uint32_t*) (mboot_info->mods_addr + 4); // Адрес конца ramdisk
    qemu_log("initrd_beg = %x initrd_end = %x",
        initrd_beg, initrd_end              // Вывод информации о адресах ramdisk в отладчик
    );

    bootScreenPaint("Configuring the Virtual Memory Manager...");
    vmm_init();                             // Инициализация менеджера виртуальной памяти

    kheap_init();                           // Инициализация кучи ядра

    init_vbe(mboot_info);                   // Активация графики 1024x768

    bootScreenPaint("Setting up a virtual file system...");
    vfs_init();                             // Инициализация виртуальной файловой системы


    bootScreenPaint("Initializing a virtual disk...");
    initrd_init(initrd_beg, initrd_end);    // Инициализация ramdisk
    bootScreenLazy(true);

    bootScreenPaint("SysApiApps Configurate...");
    syscall_init();                         // Инициализация системного api для программ

    bootScreenPaint("Installing the Keyboard Driver...");
    keyboard_install();                     // Установка драйвера клавиатуры

    bootScreenPaint("Setting the Programmable Interval Timer (PIT)...");
    timer_install();                        // Установка PIT

    bootScreenPaint("Identifying PCI Devices...");
    pci_init();                             // Установка драйвера PCI

    if (rtl8139_load){
        // Загружать ли драйвеп RTL8139
        bootScreenPaint("Installing the RTL-8139 network card driver...");
        unit_test(RTL8139_init());              // Тестируем драйвер RTL8139
    }

    vfs_mount_list();                       // Выводим список корня VFS

    bootScreenPaint("Determining the device's processor...");
    detect_cpu(1);


    bootScreenClose(0x000000,0xFFFFFF);

    if (autotshell){
        // Автоматический запуск TShell
        run_elf_file("/initrd/apps/tshell", 0, 0);
    }

    tty_printf("SynapseOS kernel version: %d.%d.%d, Built: %s\n\n",
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,    // Версия ядра
        __TIMESTAMP__                                   // Время окончания компиляции ядра
    );

    char * file_motd = "/initrd/etc/motd";
    FILE* motd = fopen(file_motd,"r");
    if (ferror(motd) == 0){
        char * buffer2 = fread(motd);
        fclose(motd);
        tty_printf("%s\n",buffer2);
    }

    struct synapse_time TIME = get_time();
    tty_printf("Current datetime is: %d/%d/%d %d:%d:%d\n", TIME.day, TIME.month,
    							TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    tty_puts("Experimental font demo: \xFF\x01 \xFF\x02 \xFF\x03 \xFF\x04 \xFF\x05 \xFF\x06 \xFF\x07 \xFF\x08 \xFF\x09 \xFF\x0A \xFF\x0B \xFF\x0C\n");
    tty_puts("Colors: \xFF\x0D\xFF\x0E\xFF\x0F\n");
    tty_puts_color("Circles: [\xFF\x10][\xFF\x11]\n", 0, 0xFF0000);

    //ATA_Drive_t *drive;
    //ATA_Detect(&drive);

    /* Перед тем как раскомментировать, хорошо подумайте, это создает громкий шум вместо звука
    sb16_init();

    if(vfs_exists("/initrd/res/sound.wav")) {
        tty_printf("EXISTS!\n");

        int fsize = vfs_get_size("/initrd/res/sound.wav");
        char* fdat = kheap_malloc(fsize);
        memset(fdat, 0, fsize);
        tty_printf("FILE SIZE: %d\n", fsize);

        vfs_read("/initrd/res/sound.wav", 0, fsize, fdat);
        tty_printf("PREDATA: %d\n", fdat[1]);

        sb16_play_audio(fdat, 44100, 1, 0, 0, fsize);

        kheap_free(fdat);
    }
    */

    duke_draw_from_file("/initrd/res/SynapseOSLogo.duke", getWidthScreen()-100, 40);

    shell();                                // Активация терминала
    // Работает, но надо доделать
    //run_driver_thread("/initrd/sys/shell.sea");
    //run_driver_thread("/initrd/sys/user.sea");
    //run_driver_thread("/initrd/sys/interface.sea");
    //run_driver_thread("/initrd/sys/kernel.elf");
}

