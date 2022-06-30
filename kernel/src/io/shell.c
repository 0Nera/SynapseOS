#include <kernel.h>
#include <libk/string.h>


char current_dir[256] = "/initrd/apps/";


void shell() {
    tty_setcolor(COLOR_ALERT);
    tty_printf("\nUse \"help\" command to get info about commands.");

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
        } else if (strcmp(cmd, "help") == 0) {
            tty_printf("Commands:\n" \
                        "\t\t->help                |get list of commands\n" \
                        "\t\t->cat   <filename>    |open file to read\n" \
                        "\t\t->cd    <folder>      |open folder\n" \
                        "\t\t->./<file>            |run programm in current folder\n" \
                        "\t\t->sbf   <code>        |run sbf programm\n" \
                        "\t\t->ls                  |list of files\n" \
                        "\t\t->sysinfo             |information about system\n" \
                        "\t\t->pcilist             |list of pci devices\n" \
                        "\n" 
                        );
        } else if (strlen(cmd) > 4 && strncmp(cmd, "cat ", 4) == 0) {
            char fname[256];

            char *tok = (char *)strtok(cmd, " ");
            
            tok = (char *)strtok(0, " "); // tok - имя файла

            if (fname != 0) {
                cat(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("cat: incorrect argument\n");
            }
        } else if (strlen(cmd) > 3 && strncmp(cmd, "cd ", 3) == 0) {
            char dname[256];
            
            char *tok = (char *)strtok(cmd, " ");
            
            tok = (char *)strtok(0, " "); // tok - now is dirname

            if (dname != 0) {
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
        } else if (strlen(cmd) > 4 && strncmp(cmd, "sbf  ", 4) == 0) {
            char fname[256];

            char *tok = (char *)strtok(cmd, " ");

            tok = (char *)strtok(0, " "); // tok - имя файла

            if (fname != 0) {
                sbf(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("sbf: incorrect argument\n");
            }
        } else if (strlen(cmd) > 2 && strncmp(cmd, "./", 2) == 0) {
            char fname[256];

            char *tok = (char *)strtok(cmd, "/");

            tok = (char *)strtok(0, "/"); // tok - имя файла

            if (fname != 0) {
                char temp[256] = {0};
                strcpy(temp, current_dir);
                strcat(temp, tok);
                run_elf_file(temp);

            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("run: incorrect argument\n");
            }
        } else {
            char fname[256];

            char *tok = (char *)strtok(cmd, "/");

            tok = (char *)strtok(0, "/"); // tok - имя файла

            if (fname != 0) {
                char temp[256] = {0};
                strcpy(temp, current_dir);
                strcat(temp, tok);
                run_elf_file(temp);

            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("run: incorrect argument\n");
            }
        }
    }
}


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
        int32_t res = vfs_read(fname, 0, fsize, buf);
        (void)res;

        buf[fsize] = '\0';

        tty_printf("%s:\n\n%s\n", fname, buf);

        kheap_free(buf);
    }
}


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