// From https://github.com/soumithrib/SB16-Driver/

#ifndef _SB16_H
#define _SB16_H

#include <libk/stdint.h>

#define SB16_IRQ_LINE       0x05
#define SB16_BASE_PORT      0x220
#define SB16_MIXR_PORT      0x224
#define SB16_RESET_PORT     0x226
#define SB16_READ_PORT      0x22A
#define SB16_WRITE_PORT     0x22C
#define SB16_POLL_PORT      0x22E
#define SB16_POLL_PORT_16   0x22F
#define ISR_IDX             0x82

#define DSP_OUT_RATE_CMD    0x41
#define DSP_BCOMMAND        0xB6
#define DSP_BMODE           0x30
#define EXIT_AUTO_DMA       0xD9

#define DMA_BASE_ADDR       0xC4
#define DMA_COUNT_PORT      0xC6
#define DMA_MASK_PORT       0xD4
#define DMA_MODE_PORT       0xD6
#define DMA_CLR_PTR_PORT    0xD8
#define DMA_PAGE_PORT       0x8B
#define DMA_START_MASK      0x01
#define DMA_STOP_MASK       0x05
#define DMA_MODE            0x59

#define TWOTO16             65536
#define BUF_RDY_VAL         128
#define SUCCESS_VAL         0xAA
#define _16B_MODE           0x2
#define SAMPLE_RATE_OUT_CMD 0x41
#define WAITLOOP            100

#define WAV_MAGIC_LOC       8
#define WAV_FORMAT_LOC      20
#define WAV_NCHANNELS_LOC   22
#define SAMPLE_RATE_LOC     24
#define BPSAMPLE_LOC        34
#define WAV_MAGIC           0x57415645
#define NCHANNELS           2
#define _16BITS             16

#define IBLOCK_SIZE         44

#define BUF_DIM             2
#define BUF_SIZE            (65536 / 2)

int sb16_init();
int32_t _sb16_play(short sample_rate, short buf_offset, char page);
int sb16_copy_status();
int sb16_shutdown();
//void sb16_interrupt(struct regs* r);


#endif
