/*
    Ядро SynapseOS

    Ядро является главным модулем операционной системы, без него невозможна работа SynapseOS.
*/

#include <kernel.h>


int32_t os_mode = 1; // 0 - мало ОЗУ, 1 - обычный режим, 2 - режим повышенной производительности, 3 - сервер


/*!
	\brief Входная точка ядра SynapseOS
	\warning Отсутствует проверка multiboot!
*/
void kernel(int32_t magic_number, struct multiboot_info *mboot_info) {
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
    log("initrd_beg = %x initrd_end = %x", 
        initrd_beg, initrd_end              // Вывод информации о адресах ramdisk в отладчик
        );

    vmm_init();                             // Инициализация менеджера виртуальной памяти

    kheap_init();                           // Инициализация кучи ядра

    init_vbe(mboot_info);                   // Активация графики 1024x768

    vfs_init();                             // Инициализация виртуальной файловой системы

    initrd_init(initrd_beg, initrd_end);    // Инициализация ramdisk

    syscall_init();                         // Инициализация системного api для программ

    keyboard_install();                     // Установка драйвера клавиатуры

    timer_install();

    pci_init();                             // Установка драйвера PCI

    ata_init();                             // Установка драйвера ATA

    unit_test(RTL8139_init());

    vfs_mount_list();

    // while(1){
    //     asm volatile("hlt");
    // }
    //log("task: %d", create_task());

    init_task_manager();
    log("proc");
    log("thread_create");
    
    while(1){asm volatile("hlt");}
    shell();                                // Активация терминала


    // Работает, но надо доделать 
    //run_driver_thread("/initrd/sys/shell.sea");
    //run_driver_thread("/initrd/sys/user.sea");
    //run_driver_thread("/initrd/sys/interface.sea");
    //run_driver_thread("/initrd/sys/kernel.elf");
}
