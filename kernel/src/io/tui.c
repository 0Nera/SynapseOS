#include <kernel.h>
#include <libk/string.h>

char current_dir[256] = "/initrd/apps/";

void init_tui(uint8_t *text) {
    for (int32_t i = 0; i < VESA_WIDTH; i += 8) {
        for (int c = 0; c <= strlen(text); c++) draw_vga_character(text[c], i, 0, 0xFF5555, 0x0000AA, true);
    }
    for (int32_t i = 0; i < VESA_WIDTH; i += 8) {
        for (int32_t j = 16; j < VESA_HEIGHT; j += 16) {
            draw_vga_character(" ", i, j, 0x0000AA, 0x0000AA, true);
        }
    }
}

void tui() {
    while (1) {

    }
}