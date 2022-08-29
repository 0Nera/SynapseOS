/**
 * @file kernel.c
 * @author Арен Елчинян (a2.dev@yandex.com), Никита Пиминов (github.com/pimnik98)
 * @brief Входная точка консоли, Synapse Command Line Interface
 * @version 0.0.3
 * @date 2022-08-28
 * @copyright Copyright Арен Елчинян (c) 2022
 */
#include <kernel.h>
#include <libk/string.h>
#include <io/imaging.h>

char current_dir[256] = "/initrd/apps/";
char* whoami = "root";
char* pcname = "oem";
char* cmd = "";


/**
 * @brief Функция выводит окно о проекте
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_about(uint32_t c,char* v[]){
    tty_printf("SynapseOS is a simple x86 C operating system with a well-documented kernel.");
    return 0;
}
/**
 * @brief Функция выводит экран справки
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_help(uint32_t c,char* v[]){
    tty_printf("Commands:\n" \
        "\t->help                | get list of commands\n" \
        "\t->cls                 | clean screen\n" \
        "\t->cat   <filename>    | open file to read\n" \
        "\t->cd    <folder>      | open folder\n" \
        "\t->./<file>            | run programm in current folder\n" \
        "\t->ls                  | list of files\n" \
        "\t->sysinfo             | information about system\n" \
        "\t->pcilist             | list of pci devices\n" \
        "\t->cpuinfo             | info cpu\n" \
        "\t->reboot              | reboot device\n" \
        "\t->shutdown            | shutdown device\n" \
        "\t->view   <filename>   | shows an image\n" \
        "\n"
    );
    return 0;
}

uint32_t cmd_cpuinfo(uint32_t c,char* v[]){
    detect_cpu(0);
    return 0;
}
/**
 * @brief Функция позволяет отчистить экран
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_cls(uint32_t c,char* v[]){
    clean_screen();
    return 0;
}

/**
 * @brief Функция перезагружающая устройство
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_reboot(uint32_t c,char* v[]){
    tty_printf("REBOOT NOW!\n");
    sleep_ms(100);
    reboot();
    return 0;
}
/**
 * @brief Функция выключающая устройство
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_shutdown(uint32_t c,char* v[]){
    tty_printf("SHUTDOWN NOW!\n");
    sleep_ms(100);
    shutdown();
    return 0;
}
/**
 * @brief Функция выводит список PCI-устройств
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_pcilist(uint32_t c,char* v[]){
    tty_printf("PCI devices:\n");
    checkAllBuses();
    return 0;
}
/**
 * @brief Функция выодит инфу
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_sysinfo(uint32_t c,char* v[]){
    sysinfo();
    return 0;
}
/**
 * @brief Функция выводящая список файлов
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_ls(uint32_t c,char* v[]){
    initrd_list(0, 0);
    return 0;
}
/**
 * @brief Функция выводящая содержимое файла
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_cat(uint32_t c,char* v[]){
    if (c == 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [CAT] You must specify the directory to which you want to change the path.\n");
        return 1;
    }
    FILE* cat_file = fopen(v[1],"r");
    if (ferror(cat_file) != 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [CAT] Could not find file `%s`. Check if the path you entered is correct.\n",v[1]);
        return 2;
    }
    char * buffer = fread(cat_file);
    fclose(cat_file);
    tty_printf("%s\n",buffer);
    return 0;
}
/**
 * @brief Функция позволяющая установить текущию папку
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_cd(uint32_t c,char* v[]){
    if (c == 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [CD] You must specify the directory to which you want to change the path.\n");
        return 1;
    }
    char* dname = v[1];
    if (dname[0] != '/') {
        char temp[256];
        strcpy(temp, current_dir);
        temp[strlen(temp) - 1] = 0;
        strcat(temp, dname);
        temp[strlen(temp) - 1] = 0;
        temp[strlen(temp) - 1] = 0;
        strcpy(dname, temp);
    }
    if (dname[strlen(dname) - 1] != '/') {
        strcat(dname, "/");
    }
    if (vfs_exists(dname) && vfs_is_dir(dname)) {
        strcpy(current_dir, dname);
    } else {
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [CD] Sorry, but the `%s` folder was not found. Check if the path you entered is correct.\n",v[1]);
        return 1;
    }
    return 0;
}
/**
 * @brief Функция позволяющая узнать и установить имя пк
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_hostname(uint32_t c,char* v[]){
    if (c == 0){
        tty_printf("%s\n",pcname);
        return 0;
    } else {
        pcname = v[1];
        tty_printf("%s\n",pcname);
        return 0;
    }
}

/**
 * @brief Функция позволяющая узнать имя пользователя ОС
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_whoami(uint32_t c,char* v[]){
    tty_printf("%s\n",whoami);
    return 0;
}

/**
 * @brief Функция просмотра изображения
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_view(uint32_t c,char* v[]){
    if (c == 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [VIEW] You must specify the path to display the image.\n");
        return 1;
    }
    FILE* c_view = fopen(v[1],"r");
    if (ferror(c_view) != 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [VIEW] Could not find file `%s`. Check if the path you entered is correct.\n",v[1]);
        return 2;
    }
    struct DukeImageMeta* data = duke_get_image_metadata(v[1]);
    if(data != 0) {
        duke_draw_from_file(v[1], getWidthScreen() - data->width - 8, 0);
        return 0;
    }else{
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [VIEW] There were problems opening the file `%s`. This file may not be in Duke format.\n",v[1]);
        return 3;
    }
    return 0;
}

uint32_t cmd_sbf(uint32_t c,char* v[]){
    if (c == 0){
        tty_printf("[CMD] [EXEC] To start batch processing, you must specify its argument.\n");
        return 1;
    }
    char fname[256] = {0};
    char *tok = (char *)strtok(cmd, " ");
    tok = (char *)strtok(0, " "); // tok - имя файла
    if (fname[0] == 0) {
        sbf(tok);
        return 0;
    } else {
        tty_setcolor(COLOR_ERROR);
        tty_printf("sbf: incorrect argument\n");
        return 1;
    }
}

/**
 * @brief Функция выполнения программы
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_exec(uint32_t c,char* v[]){
    if (c == 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [EXEC] To execute this command, you must specify the name of the file to run.\n");
        return 1;
    }
    char temp[256] = {0};
    strcpy(temp, current_dir);
    strcat(temp, v[1]);
    FILE* elf_exec = fopen(temp,"r");
    if (ferror(elf_exec) != 0){
        tty_setcolor(COLOR_ERROR);
        tty_printf("[CMD] [EXEC] The program `%s` was not found in this directory.\n",v[1]);
        return 2;
    }
    //elf_info(temp);
    run_elf_file(temp, 0, 0);
    return 0;
}

/**
 * @brief Функция откладки
 * @warning Функция сделана для консоли
 *
 * @param uint32_t c - Кол-во аргументов
 * @param char* v[] - Аргементы
 *
 * @return uint32_t - Результат работы
 */
uint32_t cmd_debug(uint32_t c,char* v[]){
    for(int i = 0; c >= i; i++){
        qemu_log("[CMD] argc: %d => argv: %s",i,v[i]);
    }
}

/**
 * @brief Входная точка консоли
 *
 */
void shell() {
    changeStageKeyboard(1);
    tty_setcolor(COLOR_ALERT);
    tty_printf("\nUse \"help\" command to get info about commands.\n");

    tty_setcolor(COLOR_TEXT);

    while (1) {
        tty_setcolor(COLOR_SYS_TEXT);
        tty_printf("\n%s@%s:",whoami,pcname);
        tty_setcolor(COLOR_SYS_PATH);
        tty_printf("%s>", current_dir);

        tty_setcolor(COLOR_TEXT);

        char* ncmd = keyboard_gets();

        if (strlen(ncmd) == 0) {
            continue;
        } else if (strlen(ncmd) > 256) {
            tty_setcolor(COLOR_ERROR);
            tty_printf("\nERROR: limit 256 char's!");
            continue;
        }
        qemu_log("[CMD] '%s'",ncmd);
        uint32_t argc = str_cdsp(ncmd," ");
        char* argv[128] = {0};
        str_split(ncmd,argv," ");
        cmd = ncmd;
        tty_printf("\n");
        for(int i = 0; argc >= i; i++){
            qemu_log("[CMD] '%s' => argc: %d => argv: %s",ncmd,i,argv[i]);
        }

        if (strcmpn(argv[0],"help")){
            cmd_help(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"debug")){
            tty_printf("debug\n");
            cmd_debug(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"run")){
            cmd_exec(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"view")){
            cmd_view(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"cd")){
            cmd_cd(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"cat")){
            cmd_cat(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"cls")){
            cmd_cls(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"reboot")){
            cmd_reboot(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"shutdown")){
            cmd_shutdown(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"pcilist")){
            cmd_pcilist(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"sysinfo")){
            cmd_sysinfo(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"ls")){
            cmd_ls(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"cpuinfo")){
            cmd_cpuinfo(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"about")){
            cmd_about(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"whoami")){
            cmd_whoami(argc,argv);
            continue;
        } else if (strcmpn(argv[0],"hostname")){
            cmd_hostname(argc,argv);
            continue;
        } else {
            char* run[2] = {0};
            run[1] = argv[0];
            cmd_exec(1,run);
            continue;
        }
    }
}


/**
 * @brief Вывод информации о системе
 * 
 */
void sysinfo(){
    tty_printf("                       ........--........        SynapseOS by Aren Elchinyan\n");
    tty_printf("                       ....+***:**+*....         Arch %s\n", ARCH_TYPE);
    tty_printf("                         .**.......**....        Ticks: %d\n", timer_get_ticks());
    tty_printf("                       ...**.......**...                        \n");
    tty_printf("                        ..:**.....-**....                       \n");
    tty_printf("                       . ...+******.....                        \n");
    tty_printf("                        .  ...***...                            \n");
    tty_printf("                           ...:**..                             \n");
    tty_printf("                            ..+**...... .                       \n");
    tty_printf("                            ..+*+.......                        \n");
    tty_printf("                    . ........-******:.. ...                    \n");
    tty_printf("                    ....**..........+**-... .                   \n");
    tty_printf("                     .-**....:***+....***....                   \n");
    tty_printf("                    ..**....********...**...                    \n");
    tty_printf("                   ...**...+********...**-.                     \n");
    tty_printf("                    ..+*-...*******:...**...                    \n");
    tty_printf(".  ...........  ......***-....+**-....***-.... ....... ......   \n");
    tty_printf("  ................-*****+**-........***-+***:.....*****:....    \n");
    tty_printf("....********...:****......+*********+......+*****+:....+*+..    \n");
    tty_printf("..***......***+*-...  . . ............  ......:**........*+.    \n");
    tty_printf(".**.. . .. .-**.         ..   .  .    ...  ...**.... ....**.    \n");
    tty_printf("**+.    . ...**..                           . .+*.......-*+..   \n");
    tty_printf(".+*..........**.                            ....+**:..****..    \n");
    tty_printf(".***.......-**..                            .......-+*.......   \n");
    tty_printf("...+********+...                            ..  .  .   .        \n");
    tty_printf("................                                                  ");
}


/**
 * @brief Интерпритация языка SBF
 * 
 * @param src - код
 */
void sbf(char *src){
    char buffer[30000] = {0};
    int32_t cursor = 0, loop = 0, current_char = 0;

    for (int32_t i = 0; src[i] != 0; i++) {
        switch (src[i]) {
            case '+':
                buffer[cursor]++;
                break;
            case '-':
                buffer[cursor]--;
                break;
            case '>':
                cursor++;
                break;
            case '<':
                cursor--;
                break;
            case '.':
                tty_putchar(buffer[cursor]);
                break;
            case ',':
                buffer[cursor] = keyboard_getchar();
                break;
            case '[':
                break;
            case ']':
                if (buffer[cursor]) {
                    loop = 1;

                    while ( loop > 0){
                        current_char = src[--i];
                        if (current_char == '['){
                            loop--;
                        } else if (current_char == ']'){
                            loop++;
                        }
                    }
                }
                break;
            case '0':
                buffer[cursor] = 0;
                break;
            case '1':
                buffer[cursor] = 1;
                break;
            case '2':
                buffer[cursor] += 2;
                break;
            case '3':
                buffer[cursor] += 3;
                break;
            case '4':
                buffer[cursor] += 4;
                break;
            case '5':
                buffer[cursor] += 5;
                break;
            case '6':
                buffer[cursor] += 6;
                break;
            case '7':
                buffer[cursor] += 7;
                break;
            case '8':
                buffer[cursor] += 8;
                break;
            case '9':
                buffer[cursor] += 9;
                break;
            
            default:
                break;
        }
    }
}
