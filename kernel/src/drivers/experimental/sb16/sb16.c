// From https://github.com/soumithrib/SB16-Driver/

#include <drivers/experimental/sb16/sb16.h>

volatile int32_t in_use = 0;
volatile int32_t int_flag = 1;

int8_t buffer[BUF_DIM][BUF_SIZE];

int32_t sb16_reset();
uint8_t dsp_read();
void dsp_write(uint8_t command);
void dsp_init(uint16_t sample_rate, uint8_t bcommand, uint8_t bmode, uint16_t block_length);
void dma_init(uint16_t buf_offset, uint16_t buf_length, uint8_t buf_page);
//void sb16_interrupt(struct regs* r);
uint8_t lo_byte(uint16_t word);
uint8_t hi_byte(uint16_t word);

void sb16_interrupt(struct regs* r) {
    asm volatile("pushal\ncli\n");
    int_flag = !int_flag;
    inb(SB16_POLL_PORT_16);
    pic_send_eoi(SB16_IRQ_LINE);
    asm volatile("pushal\nsti\n");
    asm volatile("          \n\
                  popal     \n\
                  leave     \n\
                  iret      \n\
                 ");
}

int32_t sb16_init() {
    // enable_irq(SB16_IRQ_LINE);
    register_interrupt_handler(SB16_IRQ_LINE, sb16_interrupt);
    if (in_use) {
        tty_printf("Another process is using the SB16. Terminate it and try again.\n");
        return -1;
    }
    if (sb16_reset() == -1) {
        tty_printf("SB16 initialization failed. Check hardware.\n");
        return -1;
    }
    
    /*
    sample_rate = *((uint16_t*)(info_block + SAMPLE_RATE_LOC));
    buf_offset = ((uint32_t)buffer >> 1) % TWOTO16;
    buf_page = (uint32_t)buffer >> _16BITS;
    */
}

int32_t _sb16_play(short sample_rate, short buf_offset, char page) {
    dma_init(buf_offset, BUF_SIZE - 1, page);
    dsp_init(sample_rate, DSP_BCOMMAND, DSP_BMODE, (BUF_SIZE / BUF_DIM) - 1);

    in_use = 1;
    int_flag = 1;
}

int32_t sb16_copy_status() {return int_flag;}

int32_t sb16_shutdown() {
    sb16_reset();
    in_use = 0;
    int_flag = 1;
    return 0;
}

int32_t sb16_reset() {
    int i;
    outb(1, SB16_RESET_PORT);
    for (i = 0; i < TWOTO16; i++) asm volatile("");
    outb(0, SB16_RESET_PORT);
    i = WAITLOOP;
    while ((dsp_read() != SUCCESS_VAL) && i--);
    i--;
    return i;
}

uint8_t dsp_read() {
    while (!(inb(SB16_POLL_PORT) & BUF_RDY_VAL));
    return (uint8_t) inb(SB16_READ_PORT);
}

void dsp_write(uint8_t command) {
    while (inb(SB16_WRITE_PORT) & BUF_RDY_VAL);
    outb(command, SB16_WRITE_PORT);
}

void dsp_init(uint16_t sample_rate, uint8_t bcommand, uint8_t bmode, uint16_t block_length) {
    dsp_write(DSP_OUT_RATE_CMD);
    dsp_write(hi_byte(sample_rate));
    dsp_write(lo_byte(sample_rate));
    dsp_write(bcommand);
    dsp_write(bmode);
    dsp_write(lo_byte(block_length));
    dsp_write(hi_byte(block_length));
}

void dma_init(uint16_t buf_offset, uint16_t buf_length, uint8_t buf_page) {
    outb(DMA_STOP_MASK, DMA_MASK_PORT);
    outb(0, DMA_CLR_PTR_PORT);
    outb(DMA_MODE, DMA_MODE_PORT);
    outb(lo_byte(buf_offset), DMA_BASE_ADDR);
    outb(hi_byte(buf_offset), DMA_BASE_ADDR);
    outb(lo_byte(buf_length), DMA_COUNT_PORT);
    outb(hi_byte(buf_length), DMA_COUNT_PORT);
    outb(buf_page, DMA_PAGE_PORT);
    outb(DMA_START_MASK, DMA_MASK_PORT);
}

uint8_t lo_byte(uint16_t word) {
    uint8_t word_ptr[BUF_DIM];
    *((uint16_t*)word_ptr) = word;
    return word_ptr[0];
}

uint8_t hi_byte(uint16_t word) {
    uint8_t word_ptr[BUF_DIM];
    *((uint16_t*)word_ptr) = word;
    return word_ptr[1];
}
