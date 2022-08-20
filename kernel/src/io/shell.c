#include <kernel.h>
#include <libk/string.h>
#include <io/imaging.h>

//#define PIM_DEBUG

char current_dir[256] = "/initrd/apps/";

/**
 * @brief Входная точка консоли
 * 
 */
void shell() {
    changeStageKeyboard(1);
    tty_setcolor(COLOR_ALERT);
    tty_printf("\nUse \"help\" command to get info about commands.\n");

    tty_setcolor(COLOR_TEXT);
    #ifdef PIM_DEBUG
    // Задаем путь
    char * filename = "/initrd/chip.txt";
    // Открываем файл (режим работы пока не важно, так как особо не проверяется)
    FILE* debug_readme = fopen(filename,"r");
    // Получаем размер файла
    uint32_t lSize = fsize(debug_readme);
    // Получаем содержимое файла
    char * buffer = fread(debug_readme);
    // Закончили работу с файлом
    fclose(debug_readme);
    // Выводим текст
    tty_printf("Debug stdio.c:\n\tPath: %s\n\tSize: %d\n\tText: %s\n",filename,lSize,buffer);

    // Задаем путь, но к файлу который не работает
    char * filename2 = "/initrd/readm.txt";
    // Открываем файл (режим работы пока не важно, так как особо не проверяется)
    FILE* debug_readme2 = fopen(filename2,"r");
    if (ferror(debug_readme2) != 0){
        (perror(debug_readme2,"Error"));
        fdebuginfo(debug_readme2);
    } else {
        // Получаем размер файла
        uint32_t lSize2 = fsize(debug_readme2);
        // Получаем содержимое файла
        char * buffer2 = fread(debug_readme2);
        // Закончили работу с файлом
        fclose(debug_readme2);
        // Выводим текст
        tty_printf("Debug stdio.c:\n\tPath: %s\n\tSize: %d\n\tText: %s\n",filename2,lSize2,buffer2);
    }
    #endif

    while (1) {
        tty_setcolor(COLOR_SYS_TEXT);
        tty_printf("\nROOT ");
        tty_setcolor(COLOR_SYS_PATH);
        tty_printf("%s>", current_dir);

        tty_setcolor(COLOR_TEXT);
        char *cmd = keyboard_gets();

        if (strlen(cmd) == 0) {
            continue;
        } else if (strlen(cmd) > 256) {
            tty_setcolor(COLOR_ERROR);
            tty_printf("\nERROR: limit 256 char's!");
            continue;
        }

        tty_printf("\n");

        if (strcmp(cmd, "about") == 0) {
            tty_printf("SynapseOS is a simple x86 C operating system with a well-documented kernel.");
        } else if (strcmp(cmd, "reboot") == 0) {
            tty_printf("REBOOT NOW!\n");
            sleep_ms(100);
            reboot();
        } else if (strcmp(cmd, "shutdown") == 0) {
            tty_printf("SHUTDOWN NOW!\n");
            sleep_ms(100);
            shutdown();
        } else if (strcmp(cmd, "tui") == 0) {
            tui();
        } else if (strcmp(cmd, "cls") == 0) {
            clean_screen();
        } else if (strcmp(cmd, "cpuinfo") == 0) {
            detect_cpu(0);
        } else if (strcmp(cmd, "help") == 0) {
            tty_printf("Commands:\n" \
                        "\t\t->help                |get list of commands\n" \
                        "\t\t->cls                 |clean screen\n" \
                        "\t\t->cat   <filename>    |open file to read\n" \
                        "\t\t->cd    <folder>      |open folder\n" \
                        "\t\t->./<file>            |run programm in current folder\n" \
                        "\t\t->sbf   <code>        |run sbf programm\n" \
                        "\t\t->ls                  |list of files\n" \
                        "\t\t->sysinfo             |information about system\n" \
                        "\t\t->pcilist             |list of pci devices\n" \
                        "\t\t->cpuinfo             |info cpu\n" \
                        "\t\t->reboot              |reboot device\n" \
                        "\t\t->shutdown            |shutdown device\n" \
                        "\t\t->tui                 |enable tui\n" \
                        "\t\t->view   <filename>   |shows an image\n" \
                        "\t\t->mh <program> <args>\n" \
                        "\n" 
                        );
        } else if (strlen(cmd) > 4 && strncmp(cmd, "cat ", 4) == 0) {
            char fname[256] = {0};

            char *tok = (char *)strtok(cmd, " ");
            
            tok = (char *)strtok(0, " "); // tok - имя файла

            if (fname[0] == 0) {
                cat(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("cat: incorrect argument\n");
            }
        } else if (strlen(cmd) > 3 && strncmp(cmd, "cd ", 3) == 0) {
            char dname[256] = {0};
            char *tok = (char *)strtok(cmd, " ");
            tok = (char *)strtok(0, " "); // tok - now is dirname

            if (dname[0] != 0) {
                cd(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("\ncd: incorrect argument\n");
            }
        } else if (strcmp(cmd, "pcilist") == 0) {
            tty_printf("PCI devices:\n");
	        checkAllBuses();
        } else if (strcmp(cmd, "sysinfo") == 0) {
            sysinfo();
        } else if (strcmp(cmd, "ls") == 0) {
            initrd_list(0, 0);
        } else if (strlen(cmd) > 5 && strncmp(cmd, "view ", 5) == 0) {
            char fname[256] = {0};
            char *tok = (char *)strtok(cmd, " ");
            tok = (char *)strtok(0, " "); // tok - имя файла
            
            if (fname[0] == 0) {
                struct DukeImageMeta* data = duke_get_image_metadata(tok);
                if(data!=0) {
                    duke_draw_from_file(tok, getWidthScreen() - data->width - 8, 0);
                }else{
                    tty_setcolor(COLOR_ERROR);
                    tty_printf("view: failed to read an Duke image!\n");
                }
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("view: incorrect argument\n");
            }
        } else if (strlen(cmd) > 4 && strncmp(cmd, "sbf  ", 4) == 0) {
            char fname[256] = {0};

            char *tok = (char *)strtok(cmd, " ");

            tok = (char *)strtok(0, " "); // tok - имя файла

            if (fname[0] == 0) {
                sbf(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("sbf: incorrect argument\n");
            }
        } else if (strlen(cmd) > 2 && strncmp(cmd, "./", 2) == 0) {
            char fname[256] = {0};

            char *tok = (char *)strtok(cmd, "/");

            tok = (char *)strtok(0, "/"); // tok - имя файла

            if (fname[0] == 0) {
                char temp[256] = {0};
                strcpy(temp, current_dir);
                strcat(temp, tok);
                //elf_info(temp);                
                run_elf_file(temp, 0, 0);

            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("run: incorrect argument\n");
            }
        } else if (strlen(cmd) > 2 && strncmp(cmd, "d/", 2) == 0) {
            char fname[256] = {0};

            char *tok = (char *)strtok(cmd, "/");

            tok = (char *)strtok(0, "/"); // tok - имя файла

            if (fname[0] == 0) {
                char temp[256] = {0};
                strcpy(temp, current_dir);
                strcat(temp, tok);
                elf_info(temp);

                run_elf_file(temp, 0, 0);

            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("run: incorrect argument\n");
            }
        } else if(strncmp(cmd, "mh", 2)==0) {
			char* progname = kheap_malloc(256);

			char* procmd = cmd+1; // Shift it!
			tty_printf("Parsing: [%s]\n", procmd);
			
			kheap_free(progname);
        } else {
            char fname[256] = {0};

            char *tok = (char *)strtok(cmd, "/");

            tok = (char *)strtok(0, "/"); // tok - имя файла

            if (fname[0] == 0) {
                char temp[256] = {0};
                strcpy(temp, current_dir);
                strcat(temp, tok);
                //elf_info(temp);
                run_elf_file(temp, 0, 0);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("run: incorrect argument\n");
            }
        }
    }
}


/**
 * @brief Открытие папки
 * 
 * @param dname - имя папки
 */
void cd(char *dname) {
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
        tty_printf("cd: no such directory\n");
    }
}


/**
 * @brief Вывод содержимого файла
 * 
 * @param fname - имя файла
 */
void cat(char *fname) {
    if (fname[0] != '/') {
        char temp[256];

        strcpy(temp, current_dir);
        temp[strlen(temp) - 1] = 0;

        strcat(temp, fname);

        temp[strlen(temp) - 1] = 0;
        temp[strlen(temp) - 1] = 0;

        strcpy(fname, temp);
    }

    char *buf = (char*) kheap_malloc(2048);

    if (!vfs_exists(fname)) {
        tty_setcolor(COLOR_ERROR);
        tty_printf("cat: error file not found\n");
    } else {
        uint32_t fsize = vfs_get_size(fname);
        vfs_read(fname, 0, fsize, buf);

        buf[fsize] = '\0';

        tty_printf("%s:\n\n%s\n", fname, buf);

        kheap_free(buf);
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
