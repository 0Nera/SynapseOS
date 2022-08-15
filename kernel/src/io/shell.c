#include <kernel.h>
#include <libk/string.h>
#include <io/imaging.h>

//#define PIM_DEBUG
typedef struct {
    char* com;
    char* text;
    void (*Function)(char* args);

} Command;
/**
 * @brief Открытие папки
 * 
 * @param dname - имя папки
 */

char current_dir[256] = "/initrd/apps/";
char current_user[128] = "ROOT";
int commands_len = 0;
Command commands[] = {};
bool check_command_availability(char* command) {
    for(int i=0; i<=commands_len; i++) {
        if (strcmp(commands[i].com, command) == 0) {
            return false;
        }
    }
    return true;
}
void add_command(char* command, char* text, void (*function)(char* args)) {
    Command com={command, text, function};
    commands[commands_len+1] = com;
    commands_len++;
    }
void help(char* args) {};
void clean_screen_shell(char* args) {
    clean_screen();
};
void ls(char* args){
    tty_printf("Files list \n");
    initrd_list(0,0);
}
void shell_shutdown(char* args) {
    tty_printf("SHUTDOWN...");
    sleep_ms(100);
    shutdown();
}
void view_image(char* args) {
            char fname[256] = {0};
            char *tok = (char *)strtok(args, " ");
            tok = (char *)strtok(0, " "); // tok - имя файла
            
            if (fname[0] == 0) {
                struct DukeImageMeta* data = get_image_metadata(tok);
                if(data!=0) {
                    draw_from_file(tok, getWidthScreen() - data->width - 8, 0);
                }
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("view: incorrect argument\n");
            }
}
void shell_tui(char* args) {
    tui();
}
void sysinfo(char* args){
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

void pcilist(char* args) {
    tty_printf("PCI devices:\n");
    checkAllBuses();
}
void cpuinfo(char* args) {
    detect_cpu(0);
}
void shell_reboot(char* args){
    tty_printf("REBOOT NOW!\n");
    sleep_ms(100);
    reboot();
}
void run_elf(char* args) {
    char fname[256] = {0};

    char *tok = (char *)strtok(args, "/");

    tok = (char *)strtok(0, "/"); // tok - имя файла

    if (fname[0] == 0) {
        char temp[256] = {0};
        strcpy(temp, current_dir);
        strcat(temp, tok);
        //elf_info(temp);                
        run_elf_file(temp);

    } else {
        tty_setcolor(COLOR_ERROR);
        tty_printf("run: incorrect argument\n");
    }
}


void cd(char* args) {
    char xfname[256] = {0};
    char *dname = (char *)strtok(args, " ");
    dname = (char *)strtok(0, " ");
    if (dname[0] != 0) {

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
} else {
    tty_setcolor(COLOR_ERROR);
    tty_printf("cd: incorrect argument");
}
}

void cat(char *args) {
    char xfname[256] = {0};
    char *fname = (char *)strtok(args, " ");
            
    fname = (char *)strtok(0, " "); // tok - имя файла

    if (xfname[0] == 0) {
        
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
} else {
    tty_setcolor(COLOR_ERROR);
    tty_printf("cat: incorrect argument \n");
}
}

/**
 * @brief Входная точка консоли
 * 
 */
void shell() {
    add_command("help", "- output this menu", *help);
    add_command("sysinfo", "- output systeminfo", *sysinfo);
    add_command("cls", "- clean screen", *clean_screen_shell);
    add_command("cat ", " <filename> - open file to read", *cat);
    add_command("cd ", " <dirname> - open folder", *cd);
    add_command("pcilist", "- print pcilist", *pcilist);
    add_command("ls", "- list files", *ls);
    add_command("./", "<filename> - run programm in current folder", *run_elf);
    add_command("cpuinfo", "- cpu info", *cpuinfo);
    add_command("reboot", "- reboot", *shell_reboot);
    add_command("shutdown", "- shutdown", *shell_reboot);
    add_command("tui", "- open tui", *shell_tui);
    add_command("view ", "<image> - show image", *view_image);
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
        bool comrun = false;
        tty_setcolor(COLOR_SYS_TEXT);
        tty_printf("\n %s", current_user);
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
        if (strcmp(cmd, "help") == 0) {
            comrun=true;
            tty_printf("Commands\n");
            for(int i=0; i<=commands_len; i++) {
                tty_printf("\t\t->%s", commands[i].com);
                tty_printf(" %s", commands[i].text);
                tty_printf("\n");
            }
            continue;
        }
        for(int i=0; i<=commands_len; i++) {
            if (strncmp(cmd, commands[i].com, strlen(commands[i].com)) == 0) {
                commands[i].Function(cmd);
                comrun=true;
                break;
            }
        }


    if (!comrun) {
        run_elf(cmd);
    }        
    }
}
