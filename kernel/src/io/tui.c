#include <kernel.h>
#include <io/tty.h>
#include <libk/string.h>

char current_dir[256] = "/initrd/apps/";

int outp = 0;

void init_tui(const char *txt) {
    for (int32_t i = 0; i < VESA_WIDTH; i += 8) {
        for (int32_t j = 0; j < VESA_HEIGHT; j += 16) {
            draw_vga_character(" ", i, j, VESA_BLUE, VESA_BLUE, true);
        }
    }
    reset_pos();
    tty_setcolor(VESA_WHITE);
    tui(txt);
}

void tui(const char *teext) {
    uint32_t menuentry = 1;
    uint32_t podmenu = 1;
    uint32_t option = 1;
    while (1) {
        reset_pos();
        tty_printf(teext);
        //////////////////////////////////// KEYS //////////////////////////////////////
        uint32_t key = keyboard_getscancode();
        if (key == 77) {
            for (int32_t j = 0; j < VESA_WIDTH; j += 8) {
                for (int32_t i = 0; i < 64; i += 8) {
                    draw_vga_character(" ", j, i, VESA_BLUE, VESA_BLUE, true);
                }
            }
            menuentry++;
        } else if (key == 75) {
            for (int32_t j = 0; j < VESA_WIDTH; j += 8) {
                for (int32_t i = 0; i < 64; i += 8) {
                    draw_vga_character(" ", j, i, VESA_BLUE, VESA_BLUE, true);
                }
            }
            menuentry--;
        }
        if (key == 80) {
            for (int32_t j = 0; j < VESA_WIDTH; j += 8) {
                for (int32_t i = 0; i < 64; i += 8) {
                    draw_vga_character(" ", j, i, VESA_BLUE, VESA_BLUE, true);
                }
            }
            podmenu++;
        } else if (key == 72) {
            for (int32_t j = 0; j < VESA_WIDTH; j += 8) {
                for (int32_t i = 0; i < 64; i += 8) {
                    draw_vga_character(" ", j, i, VESA_BLUE, VESA_BLUE, true);
                }
            }
            podmenu--;
        }
        //////////////////////////////////// FUNC  //////////////////////////////////////
        if (key == 28) {
            if (option == 1) {
                reboot();
            }
            if (option == 2) {
                shutdown();
            }
            if (option == 3) {
                cls(VESA_BLACK);
                shell();
            }
            if (option == 4) {
                run_elf_file("/initrd/apps/NeraMath.elf");
            }
            if (option == 5) {
                //run_elf_file("/initrd/apps/snake.elf");
            }
            if (option == 6) {
                //run_elf_file("/initrd/apps/dog.elf");
            }
        }
        ////////////////////////////////// ENTRIES /////////////////////////////////////
        if (menuentry == 1) {
            for (int32_t i = 0; i < 48; i += 8) {
                for (int32_t j = 0; j < 8; j += 8) {
                    draw_vga_character(" ", i, j, VESA_YELLOW, VESA_YELLOW, true);       
                }
            }
            tty_printf("\n Reboot \nShutdown\n Shell  ");
        } 
        if (menuentry == 2) {
            for (int32_t i = 48; i < 96; i += 8) {
                for (int32_t j = 0; j < 8; j += 8) {
                    draw_vga_character(" ", i, j, VESA_YELLOW, VESA_YELLOW, true);       
                }
            }
            tty_printf("\n Nera Math \n   Snake   \n    Dog    ");
        } 
        if (menuentry == 3) {
            for (int32_t i = 96; i < 176; i += 8) {
                for (int32_t j = 0; j < 8; j += 8) {
                    draw_vga_character(" ", i, j, VESA_YELLOW, VESA_YELLOW, true);       
                }
            }
            tty_printf("\n Back Color \n Text Color \n------------");
        }
        ////////////////////////////////////////////////////////////////////////////////
        if (menuentry == 1) {
            if (podmenu == 1) {
                for (int32_t i = 0; i < 64; i += 8) {
                    for (int32_t j = 16; j < 24; j += 8) {
                        draw_vga_character(" ", i, j, VESA_GREEN, VESA_GREEN, true);       
                    }
                }
                reset_pos();
                tty_printf("\n Reboot \nShutdown\n Shell  ");
                option = 1;
            } else if (podmenu == 2) {
                for (int32_t i = 0; i < 64; i += 8) {
                    for (int32_t j = 32; j < 40; j += 8) {
                        draw_vga_character(" ", i, j, VESA_GREEN, VESA_GREEN, true);       
                    }
                }
                reset_pos();
                tty_printf("\n Reboot \nShutdown\n Shell  ");
                option = 2;
            } else if (podmenu == 3) {
                for (int32_t i = 0; i < 64; i += 8) {
                    for (int32_t j = 48; j < 56; j += 8) {
                        draw_vga_character(" ", i, j, VESA_GREEN, VESA_GREEN, true);       
                    }
                }
                reset_pos();
                tty_printf("\n Reboot \nShutdown\n Shell  ");
                option = 3;
            }
        }
        if (menuentry == 2) {
            if (podmenu == 1) {
                for (int32_t i = 0; i < 88; i += 8) {
                    for (int32_t j = 16; j < 24; j += 8) {
                        draw_vga_character(" ", i, j, VESA_GREEN, VESA_GREEN, true);       
                    }
                }
                reset_pos();
                tty_printf("\n Nera Math \n   Snake   \n    Dog    ");
                option = 4;
            } else if (podmenu == 2) {
                for (int32_t i = 0; i < 88; i += 8) {
                    for (int32_t j = 32; j < 40; j += 8) {
                        draw_vga_character(" ", i, j, VESA_GREEN, VESA_GREEN, true);       
                    }
                }
                reset_pos();
                tty_printf("\n Nera Math \n   Snake   \n    Dog    ");
                option = 5;
            } else if (podmenu == 3) {
                for (int32_t i = 0; i < 88; i += 8) {
                    for (int32_t j = 48; j < 56; j += 8) {
                        draw_vga_character(" ", i, j, VESA_GREEN, VESA_GREEN, true);       
                    }
                }
                reset_pos();
                tty_printf("\n Nera Math \n   Snake   \n    Dog    ");
                option = 6;
            }
        }
        ////////////////////////////////////////////////////////////////////////////////
        if (menuentry > 3) {
            menuentry = 3;
        } 
        if (menuentry < 1) {
            menuentry = 1;
        }
        if (podmenu > 3) {
            podmenu = 3;
        } 
        if (podmenu < 1) {
            podmenu = 1;
        }
        ////////////////////////////////////////////////////////////////////////////////
        reset_pos();
        tty_printf(text); 
        sleep(250);
    }
}

void cls(uint32_t color) {
    for (int32_t i = 0; i < VESA_WIDTH; i += 8) {
        for (int32_t j = 0; j < VESA_HEIGHT; j += 16) {
            draw_vga_character(" ", i, j, color, color, true);
        }
    }
}