#include <kernel.h>


volatile uint8_t tty_feedback = 1;

size_t tty_line_fill[1024];

int tty_pos_x;
int tty_pos_y;


uint32_t tty_text_color;

/*
    vga_entry - слияние символа и цвета, для вывода
*/
uint16_t vga_entry(uint8_t c, uint8_t tty_color) {
    return (uint16_t) c | (uint16_t) tty_color << 8;
}

void tty_setcolor(int color){
    tty_text_color = color;
}

void init_vbe(multiboot_info *mboot) {
   
    svga_mode_info_t *svga_mode = (svga_mode_info_t*) mboot->vbe_mode_info;
    framebuffer_addr = (uint8_t *)svga_mode->physbase; 
    framebuffer_pitch = svga_mode->pitch;
    framebuffer_bpp = svga_mode->bpp;
    framebuffer_width = svga_mode->screen_width;
    framebuffer_height = svga_mode->screen_height;
    framebuffer_size = framebuffer_height * framebuffer_pitch;

    physical_addres frame;
    virtual_addr virt;
    for (frame = framebuffer_addr, virt = framebuffer_addr;
         frame < (framebuffer_addr + framebuffer_size/*0x00400000*//*0x002C0000 0x000F0000*/);
         frame += PAGE_SIZE, virt += PAGE_SIZE) {
        vmm_map_page(frame, virt);
    }
    qemu_putstring("VBE create_back_framebuffer\n");

    create_back_framebuffer();
}

void create_back_framebuffer() {
    //flush_tlb_entry(back_framebuffer_addr);
    back_framebuffer_addr = kheap_malloc(framebuffer_size);
    qemu_printf("back_framebuffer_addr = %x\n", back_framebuffer_addr);
    //tty_printf("init_vbe: [c0800000]->%x\n", page_table_entry_is_writable(GET_PTE(0xC0800000)));
    memset(back_framebuffer_addr, 0, framebuffer_size); //causes page fault at c0800000 when this line is placed in the end of init_vbe
}

/*
    tty_init - очистка экрана и сброс настроек tty
*/
void tty_init(struct multiboot_info *mboot_info) {
    tty_pos_y = 0;
    tty_pos_x = 0;

    tty_text_color = COLOR_SYS_TEXT;

    svga_mode_info_t *svga_mode = (svga_mode_info_t*) mboot_info->vbe_mode_info;

    framebuffer_addr = (uint8_t *)svga_mode->physbase;
    framebuffer_pitch = svga_mode->pitch;
    framebuffer_bpp = svga_mode->bpp;
    framebuffer_width = svga_mode->screen_width;
    framebuffer_height = svga_mode->screen_height;
    framebuffer_size = framebuffer_height * framebuffer_pitch;
    back_framebuffer_addr = framebuffer_addr;
}

void tty_scroll() {
    // charheight = 16???
    unsigned int num_rows = 1;
    tty_pos_y -= 17*num_rows;

    // Copy rows upwards
    uint8_t *read_ptr = (uint8_t*) back_framebuffer_addr + ((num_rows * 17) * framebuffer_pitch);
    uint8_t *write_ptr = (uint8_t*) back_framebuffer_addr;
    uint32_t num_bytes = (framebuffer_pitch * VESA_HEIGHT) - (framebuffer_pitch * (num_rows * 17)); //old: unsigned old
    memcpy(write_ptr, read_ptr, num_bytes);

    // Clear the rows at the end
    write_ptr = (uint8_t*) back_framebuffer_addr + (framebuffer_pitch * VESA_HEIGHT) - (framebuffer_pitch * (num_rows * 17));
    memset(write_ptr, 0, framebuffer_pitch * (num_rows * 17));

    //swap buffers
    memcpy(framebuffer_addr, back_framebuffer_addr, framebuffer_size);
}

void set_pixel(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || 
        x >= (int) VESA_WIDTH || 
        y >= (int) VESA_HEIGHT) {
        return;
    }

    unsigned where = x * (framebuffer_bpp / 8) + y * framebuffer_pitch;

    framebuffer_addr[where] = color;
    framebuffer_addr[where + 1] = color >> 8;
    framebuffer_addr[where + 2] = color >> 16;


    back_framebuffer_addr[where] = color & 255;
    back_framebuffer_addr[where + 1] = (color >> 8) & 255;
    back_framebuffer_addr[where + 2] = (color >> 16) & 255;

}

void set_line(int x, int y, int xe, int ye, uint32_t color){
    for (int i = x; i < xe; i++) {
        for (int j = y; j < ye; j++) {
            set_pixel(i, j, color);
        }
        
    }
    
}

/*
    tty_putchar - вывод одного символа
*/
void tty_putchar(char c) {
    
    if ((tty_pos_x + 8) >= (int)VESA_WIDTH || c == '\n') { 
        tty_line_fill[tty_pos_y] = tty_pos_x;
        tty_pos_x = 0;

        if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) { 
            tty_scroll();
        } else {
            tty_pos_y += 17;
        }
    } else {

        if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) {
            tty_scroll();
        }
        draw_vga_character(c, tty_pos_x, tty_pos_y, tty_text_color, 0x000000, 0);
        tty_pos_x += 8;
    }
}

void draw_vga_character(uint8_t c, int x, int y, int fg, int bg, bool bgon) {

    int cx, cy;
    int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    unsigned char *glyph = (uint8_t*) vgafnt + (int) c * 16;

    for (cy = 0; cy < 16; cy++) {
        for (cx = 0; cx < 8; cx++) {
            if (glyph[cy] & mask[cx]) {
                set_pixel(x + cx, y + cy, fg);
            } else if (bgon == true) {
                set_pixel(x + cx, y + cy, bg);
            }
        }
    }
}
void tty_backspace() {
    if (tty_pos_x < 8) { // Old: == 0
        if (tty_pos_y >= 17) {
            tty_pos_y -= 17;
        }
        tty_pos_x = tty_line_fill[tty_pos_y];
    } else {
        tty_pos_x -= 8;
    }
    draw_vga_character(' ', tty_pos_x, tty_pos_y, tty_text_color, 0x000000, 1);
}

/*
    tty_puts - вывод строки
*/
void tty_puts(const char c[]) {
    for (size_t i = 0; i < strlen(c); i++) {
        tty_putchar(c[i]);
    }
}


/*
    tty_putint - вывод числа
*/
void tty_putint(const int i) {
    char res[32];

    if (i < 0) {
        tty_putchar('-');
    }

    itoa(i, res);
    tty_puts(res);
    
}


/*
    tty_puthex - вывод hex чисел
*/
void tty_puthex(uint32_t i) {
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned int n, d = 0x10000000;

    tty_puts("0x");

    while((i / d == 0) && (d >= 0x10)) d /= 0x10;

    n = i;

    while( d >= 0xF ) {
        tty_putchar(hex[n/d]);
        n = n % d;
        d /= 0x10;
    }

    tty_putchar(hex[n]);
}


/*
    tty_print - строгий, форматированный вывод
*/
void tty_print(char *format, va_list args) {
    int i = 0;

    while (format[i]) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's':
                    tty_puts(va_arg(args, char*));
                    break;
                case 'c':
                    tty_putchar(va_arg(args, int));
                    break;
                case 'd':
                    tty_putint(va_arg(args, int));
                    break;
                case 'i':
                    tty_putint(va_arg(args, int));
                    break;
                case 'u':
                    tty_putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    tty_puthex(va_arg(args, uint32_t));
                    break;
                default:
                    tty_putchar(format[i]);
            }
        } else if (format[i] == 10) {
            tty_line_fill[tty_pos_y] = tty_pos_x;
            tty_pos_x = 0;

            if ((tty_pos_y + 17) >= (int)VESA_HEIGHT) { 
                tty_scroll();
            } else {
                tty_pos_y += 17;
            }
        } else if (format[i] == 9) {
            tty_pos_x += 4 * 17;
        } else {
            tty_putchar(format[i]);
        }
        i++;
    }
}


/*
    tty_printf - форматированный вывод
*/
void tty_printf(char *text, ...) {
    va_list args;
    va_start(args, text);
    tty_print(text, args);
}