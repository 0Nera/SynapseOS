#include <kernel.h>

char current_dir[256] = "/initrd/apps/";

void ksh_main() {
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
                        "->help                |get list of commands\n" \
                        "->cat   <filename>    |open file to read\n" \
                        "->cd    <folder>      |open folder\n" \
                        "->./<file>            |run .elf programm in current folder\n" \
                        "->ls                  |print list of files\n" 
                        );
        } else if (strlen(cmd) > 4 && strncmp(cmd, "cat ", 4) == 0) {
            char fname[256];

            char *tok = strtok(cmd, " ");
            
            tok = strtok(0, " "); // tok - now is filename

            if (fname != 0) {
                cat(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("cat: incorrect argument\n");
            }
        } else if (strlen(cmd) > 3 && strncmp(cmd, "cd ", 3) == 0) {
            char dname[256];
            
            char *tok = strtok(cmd, " ");
            
            tok = strtok(0, " "); // tok - now is dirname

            if (dname != 0) {
                cd(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("\ncd: incorrect argument\n");
            }
        } else if (strcmp(cmd, "ls") == 0) {
            initrd_list(0, 0);
        } else if (strlen(cmd) > 4 && strncmp(cmd, "sbf  ", 4) == 0) {
            char fname[256];

            char *tok = strtok(cmd, " ");

            tok = strtok(0, " "); // tok - now is filename

            if (fname != 0) {
                sbf(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("sbf: incorrect argument\n");
            }
        } else if (strlen(cmd) > 2 && strncmp(cmd, "./", 2) == 0) {
            char fname[256];

            char *tok = strtok(cmd, "/");

            tok = strtok(0, "/"); // tok - now is filename

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
            tty_setcolor(COLOR_ERROR);
            tty_printf("Unknown: [%s]\n", cmd);
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
        int res = vfs_read(fname, 0, fsize, buf);
        (void)res;

        buf[fsize] = '\0';

        tty_printf("%s:\n\n%s\n", fname, buf);

        kheap_free(buf);
    }
}



void sbf(char *src){
    for (int i = 0; src[i] != 0; i++) {
        switch (src[i])
        {
        case '+':
            /* code */
            break;
        
        default:
            break;
        }
    }
}