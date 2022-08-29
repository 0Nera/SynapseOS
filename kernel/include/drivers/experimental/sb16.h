#ifndef SB16_DSP_H
#define SB16_DSP_H

// Ports
#define DSP_BASE        0x220
#define DSP_MIXER_PORT  0x224
#define DSP_MIXER_DATA  0x225
#define DSP_RESET       0x226
#define DSP_READ        0x22A
#define DSP_WRITE       0x22C
#define DSP_READ_STATUS 0x22E
#define DSP_16_ACK      0x22F

// DSP Write Command
#define DSP_W_SET_TIME  0x40
#define DSP_W_SET_SR    0x41
#define DSP_W_SPK_ON    0xD1
#define DSP_W_SPK_OFF   0xD3
#define DSP_W_STOP_8    0xD0
#define DSP_W_RESUME_8  0xD4
#define DSP_W_STOP_16   0xD5
#define DSP_W_RESUME_16 0xD6
#define DSP_W_GET_VER   0xE1

#define DSP_M_MAST_VOLU 0x22
#define DSP_M_SET_IRQ   0x80

#include <io/ports.h>

char sb16_init();
char sb16_can_play_audio();
char sb16_dsp_reset();
void sb16_set_irq(char irq);
void sb16_turn_speaker_on();
// void sb16_program_dma8(char channel, int address, short length);
void sb16_program_dma16(char channel, int address, short length);
unsigned int sb16_calculate_time_constant(char channels, int sampling_rate);
void sb16_program(unsigned int sampling_rate, char channels, char eightbit, char sign, short length);
void sb16_set_master_volume(char left, char right);
void sb16_play_audio(char *data, unsigned sampling_rate, char channels, char eightbit, char sign, int length);

#endif
