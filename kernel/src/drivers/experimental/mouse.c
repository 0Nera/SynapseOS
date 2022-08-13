// From https://github.com/rgimad/EOS/blob/master/kernel/devices/mouse.c

#include <drivers/mouse.h>
#include <kernel.h>

uint8_t mouse_ready = 0;
int32_t mouse_x  = 0;
int32_t mouse_y  = 0;

uint8_t mouse_b1 = 0;
uint8_t mouse_b2 = 0;
uint8_t mouse_b3 = 0;
uint8_t mouse_b4 = 0;
uint8_t mouse_b5 = 0;

unsigned int xmax;
unsigned int ymax;

int mouse_wheel = 0;

// This shouldn't be used outside of mouse driver.
typedef struct mouse_flags_byte {
    unsigned int left_button   : 1;
    unsigned int right_button  : 1;
    unsigned int middle_button : 1;

    unsigned int always1 : 1;

    unsigned int x_sign : 1;
    unsigned int y_sign : 1;

    unsigned int x_overflow : 1;
    unsigned int y_overflow : 1;
} __attribute__((packed)) mouse_flags_byte;

struct dev_ps2m_mouse_packet {
    int16_t movement_x;
    int16_t movement_y;
    uint8_t button_l;
    uint8_t button_m;
    uint8_t button_r;
} ps2m_buffer;

void mouse_parse_packet(const char *buf, uint8_t has_wheel, uint8_t has_5_buttons) {
    mouse_flags_byte *mfb = (mouse_flags_byte*) (buf);
    if (mfb->x_overflow || mfb->y_overflow || !mfb->always1) {
        return;
    }

    int offx = (int16_t) (0xff00 * mfb->x_sign) | buf[1];
    int offy = (int16_t) (0xff00 * mfb->y_sign) | buf[2];
    mouse_x += offx;
    mouse_y -= offy;
    mouse_b1 = mfb->left_button;
    mouse_b2 = mfb->right_button;
    mouse_b3 = mfb->middle_button;
    ps2m_buffer.movement_x = offx;
    ps2m_buffer.movement_y = offy;
    ps2m_buffer.button_l = mouse_b1;
    ps2m_buffer.button_r = mouse_b2;
    ps2m_buffer.button_m = mouse_b3;

    if (has_wheel) {
        // Parse wheel (byte 3, bits 0-3)
        mouse_wheel += (char) ((!!(buf[3] & 0x8)) * 0xf8 | (buf[3] & 0x7));
        if (has_5_buttons) {
            mouse_b4 = !!(buf[3] & 0x10);
            mouse_b4 = !!(buf[3] & 0x20);
            // parse buttons 4-5 (byte 3, bits 4-5)
        }
    }
}

void mouse_handler(__attribute__((unused)) struct regs *r) {
    uint8_t status = inb(0x64);
    if ((status & 1) == 0 || (status >> 5 & 1) == 0) {
        return;
    }

    static int recbyte = 0;
    static char mousebuf[16];

    mousebuf[recbyte++] = inb(0x60);
    if (recbyte == 3) {
        recbyte = 0;
        mouse_parse_packet(mousebuf, 0, 0);

        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_x > xmax) mouse_x = xmax;
        if (mouse_y > ymax) mouse_y = ymax;
    }
}

int mouse_getx() { return mouse_x; }
int mouse_gety() { return mouse_y; }
int mouse_getb1() { return mouse_b1; }
int mouse_getb2() { return mouse_b2; }
int mouse_getb3() { return mouse_b3; }
int mouse_getb4() { return mouse_b4; }
int mouse_getb5() { return mouse_b5; }

void mouse_wait(uint8_t a_type) {
    uint32_t _time_out = 100000;
    if (a_type == 0) {
        while (_time_out--) {
            if ((inb(0x64) & 1) == 1) {
                return;
            }
        }
        return;
    } else {
        while (_time_out--) { //Signal
            if ((inb(0x64) & 2) == 0) {
                return;
            }
        }
        return;
    }
}

void mouse_write(uint8_t a_write) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, a_write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_install() {
    uint8_t _status;  //unsigned char

    // Enable the auxiliary mouse device
    mouse_wait(1);
    outb(0x64, 0xA8);

    // Enable the interrupts
    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    _status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, _status);

    // Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read(); // Acknowledge

    // Enable the mouse
    mouse_write(0xF4);
    mouse_read(); // Acknowledge

    // Set cursor coordinates in middle of the screen
    mouse_x = 0;
    mouse_y = 0;

    register_interrupt_handler(MOUSE_IDT_INDEX, &mouse_handler);
    mouse_ready = 1;
}
