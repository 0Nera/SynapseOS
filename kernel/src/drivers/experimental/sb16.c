/*    Драйвер SoundBlaster 16
 *
 *    By Andrey(David) Pavlenko aka NDRAEY
 */ 

// Нам нужно место в пределах 0x00100000 - 0x00FFFFFF
// SoundBlaster 16 работает только с ними.
// Возьмем где-нибудь адрес с длиной 8192 байт

#define LOAD        0x100000
#define LOAD_LENGTH 8192

#include <drivers/experimental/sb16.h>
#include <libk/string.h>
#include <mem/mem.h>

char can_play_audio = 0;
char* driver_memory = (char*)LOAD;

char sb16_init() {
	char audio = sb16_dsp_reset();
	if(audio==0xFFFFFFFF) {
		return 0;
	}else{
		for(int memp = 0; memp < 2; memp++) {
			int addrk = LOAD+(PAGE_SIZE*memp);
			qemu_log("SB16: Allocating at: %x\n", addrk);
			vmm_alloc_page(addrk);
		}
		can_play_audio=1;
		return 1;
	}
}

void sb16_dsp_write(char cmd) {
	while(inb(DSP_WRITE) & 128);
	outb(DSP_WRITE, cmd);
}

char sb16_can_play_audio() {
	return can_play_audio;
}

char sb16_dsp_reset() {
	outb(DSP_RESET, 1);
	for(char _ = 0; _<64; _++) {asm volatile("nop");}
	outb(DSP_RESET, 0);

	return inb(DSP_READ);
}

void sb16_set_irq(char irq) {
	outb(DSP_MIXER_PORT, 0x80);
	outb(DSP_MIXER_DATA, irq);
}

void sb16_turn_speaker_on() {
	sb16_dsp_write(0xD1);
}

/*
void sb16_program_dma8(char channel, int address, short length) {
	length -= 1;
	outb(0x0A, 4+(channel%4));
	outb(0x0C, 1);
	outb(0x0B, 0x58+(channel%4));
	char page = address>>16;
	char lop  = address%(address>>8);
	char hip  = (address>>8)%(page<<8);
	outb(0x83, page);
	outb(0x02, lop);
	outb(0x02, hip);
	char lol = length%((length>>8)<<8);
	char hil = (length>>8);
	outb(0x03, lol);
	outb(0x03, hil);
	outb(0x0A, channel%4);
}
*/

void sb16_program_dma16(char channel, int address, short length) {
	outb(0xD4, 4+(channel)); // Channel
	outb(0xD8, 1); // Flip-flop
	//outb(0xD6, 0b10110000); // 16-bit sound, no FIFO
	outb(0xD6, 0x48+(channel));
	char page = (address&0xFF0000)>>16;  // 0x[AA]BBCC
	char lop  = address&0x0000FF;        // 0xAABB[CC]
	char hip  = (address&0x00FF00)>>8;   // 0xAA[BB]CC
	outb(0x8B, page);
	outb(0xC4, lop);
	outb(0xC4, hip);
	char lol = (length&0x00FF);
	char hil = (length&0xFF00)>>8;
	outb(0xC6, lol);
	outb(0xC6, hil);
	outb(0xD4, channel);
}

unsigned int sb16_calculate_time_constant(char channels, int sampling_rate) {
	return 65536-(256000000/(channels*sampling_rate));
}

void sb16_program(unsigned int sampling_rate, char stereo, char eightbit, char sign, short length) {
	length -= 1;
	/*
	outb(DSP_WRITE, 0x40);
	outb(DSP_WRITE, sb16_calculate_time_constant(channels, sampling_rate));
	outb(DSP_WRITE, eightbit?0xC0:0xB0);
	outb(DSP_WRITE, (channels>=1?0b00100000:0)|(sign>=1?0b00010000:0));
	outb(DSP_WRITE, length&0x00FF);
	outb(DSP_WRITE, (length&0xFF00)>>8);
	*/

	sb16_dsp_write(0x40);
	// sb16_dsp_write(sb16_calculate_time_constant(channels, sampling_rate));
	sb16_dsp_write(145);
	sb16_dsp_write(eightbit?0xC0:0xB0);
	sb16_dsp_write((stereo?0b00100000:0)|(sign?0b00010000:0));
	sb16_dsp_write(length&0x00FF);
	sb16_dsp_write((length&0xFF00)>>8);
}

void sb16_set_master_volume(char left, char right) {
	outb(DSP_MIXER_PORT, DSP_M_MAST_VOLU);
	outb(DSP_MIXER_DATA, right|(left<<4));
}

void sb16_play_audio(char *data, unsigned int sampling_rate, char channels, char eightbit, char sign, int length) {
	// 1. Reset DSP
	sb16_dsp_reset();

	// 2. Load sound data to memory
	memcpy(driver_memory, data, LOAD_LENGTH);

	// 3. Set master volume
	sb16_set_master_volume(0xF, 0xF);
	// 4. Turn speaker on
	sb16_turn_speaker_on();
	// 6, 7, 8, 9, 10
	sb16_program(sampling_rate, channels, eightbit, sign, LOAD_LENGTH);

	tty_printf("DAT1: %d\nDAT2: %d\n", driver_memory[1], data[1]);

	sb16_program_dma16(channels, LOAD, LOAD_LENGTH);

	
	/*while(loaded<length-1) {
		memcpy(driver_memory, data, LOAD_LENGTH);
		sb16_program_dma16(channels, driver_memory, LOAD_LENGTH);
		loaded+=LOAD_LENGTH;
		data+=LOAD_LENGTH;
	}*/
}
