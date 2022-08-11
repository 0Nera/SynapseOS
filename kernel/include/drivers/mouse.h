#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <kernel.h>
#include <stdbool.h>
#include <stdint.h>

#define MOUSE_RESET           0xFF
#define MOUSE_RESEND          0xFE
#define MOUSE_DEFAULTS        0xF6
#define MOUSE_STREAMING_OFF   0xF5
#define MOUSE_STREAMING_ON    0xF4
#define MOUSE_SET_SAMPLE_RATE 0xF3
#define MOUSE_GET_ID          0xF2
#define MOUSE_REQUEST_PACKET  0xEB
#define MOUSE_REQUEST_STATUS  0xE9
#define MOUSE_SET_RESOLUTION  0xE8
#define MOUSE_CONTROL_PORT    0x64
#define MOUSE_CMD_SEND        0xD4
#define MOUSE_DATA_PORT       0x60

// Mouse answers
#define MOUSE_ACK             0xFA

void mouse_wait(uint8_t a_type);
void mouse_write(uint8_t a_write);
uint8_t mouse_read();
void mouse_install();

extern uint8_t mouse_ready;

extern int32_t mouse_x;
extern int32_t mouse_y;

extern uint8_t mouse_b1;
extern uint8_t mouse_b2;
extern uint8_t mouse_b3;
extern uint8_t mouse_b4;
extern uint8_t mouse_b5;

extern int mouse_wheel;

int mouse_getx();
int mouse_gety();
int mouse_getb1();
int mouse_getb2();
int mouse_getb3();
int mouse_getb4();
int mouse_getb5();

#endif // _MOUSE_H_
