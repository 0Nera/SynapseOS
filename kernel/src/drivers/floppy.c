#include <kernel.h>


static const char * drive_types[8] = {
    "none",
    "360kB 5.25\"",
    "1.2MB 5.25\"",
    "720kB 3.5\"",

    "1.44MB 3.5\"",
    "2.88MB 3.5\"",
    "unknown type",
    "unknown type"
};

static const char * status[] = { 
    0, 
    "error", 
    "invalid", 
    "drive" 
};

enum { 
    floppy_motor_off = 0, 
    floppy_motor_on, 
    floppy_motor_wait 
};
static volatile int floppy_motor_ticks = 0;
static volatile int floppy_motor_state = 0;


// Obviously you'd have this return the data, start drivers or something.
void floppy_detect_drives() {
   outb(0x70, 0x10);
   unsigned drives = inb(0x71);

   tty_printf(" - Floppy drive 0: %s\n", drive_types[drives >> 4]);
   tty_printf(" - Floppy drive 1: %s\n", drive_types[drives & 0xf]);

}

void floppy_motor() {

}
void irq_wait(){

}

void floppy_handler(struct regs *r) {
    uint32_t adr;
    asm volatile("movl %%cr2, %0" : "=r" (adr));
    qemu_printf("\nFLOPPY cr2 = %x  r->idt_index = %x eax = %x  ebx = %x" \
                "  ecx = %x  edx = %x  esp = %x  ebp = %x  eip = %x\n", 
        adr, r->idt_index, r->eax, r->ebx, 
        r->ecx, r->edx, r->esp, r->ebp, r->eip);
    qemu_printf("\nFLOPPY cr2 = %d  r->idt_index = %d eax = %d  ebx = %d" \
        "  ecx = %d  edx = %d  esp = %d  ebp = %d  eip = %d\n", 
        adr, r->idt_index, r->eax, r->ebx, 
        r->ecx, r->edx, r->esp, r->ebp, r->eip);
    
    tty_printf("\nFLOPPY cr2 = %x  r->idt_index = %x eax = %x  ebx = %x" \
        "  ecx = %x  edx = %x  esp = %x  ebp = %x  eip = %x\n", 
        adr, r->idt_index, r->eax, r->ebx, 
        r->ecx, r->edx, r->esp, r->ebp, r->eip);
    tty_printf("\nFLOPPY cr2 = %d  r->idt_index = %d eax = %d  ebx = %d" \
        "  ecx = %d  edx = %d  esp = %d  ebp = %d  eip = %d\n", 
        adr, r->idt_index, r->eax, r->ebx, 
        r->ecx, r->edx, r->esp, r->ebp, r->eip);
}

void floppy_write_cmd(int base, char cmd) {
    int i; // do timeout, 60 seconds
    for(i = 0; i < 600; i++) {
        sleep(10); // sleep 10 ms
        if(0x80 & inb(base+FLOPPY_MSR)) {
            return (void) outb(base+FLOPPY_FIFO, cmd);
        }
    }
    tty_printf("floppy_write_cmd: timeout");   
}


unsigned char floppy_read_data(int base) {

    int i; // do timeout, 60 seconds
    for(i = 0; i < 600; i++) {
        sleep(10); // sleep 10 ms
        if(0x80 & inb(base+FLOPPY_MSR)) {
            return inb(base+FLOPPY_FIFO);
        }
    }
    tty_printf("floppy_read_data: timeout");
    return 0; // not reached
}
int floppy_init(int drive) {
    floppy_detect_drives();
    register_interrupt_handler(38, &floppy_handler);
    sleep(1000);
    tty_printf("\ndata = [");
    for (int i = 256; i != 0; i--){
        tty_printf("%d", floppy_read_data(1));
    }
    tty_printf("]\n");
    return 0;
}






void floppy_check_interrupt(int base, int *st0, int *cyl) {
   
    floppy_write_cmd(base, CMD_SENSE_INTERRUPT);

    *st0 = floppy_read_data(base);
    *cyl = floppy_read_data(base);
}

// Move to cylinder 0, which calibrates the drive..
int floppy_calibrate(int base) {

    int i, st0, cyl = -1; // set to bogus cylinder

    floppy_motor(base, floppy_motor_on);

    for(i = 0; i < 10; i++) {
        // Attempt to positions head to cylinder 0
        floppy_write_cmd(base, CMD_RECALIBRATE);
        floppy_write_cmd(base, 0); // argument is drive, we only support 0

        irq_wait(floppy_irq);
        floppy_check_interrupt(base, &st0, &cyl);
       
        if(st0 & 0xC0) {
            
            tty_printf("floppy_calibrate: status = %s\n", status[st0 >> 6]);
            continue;
        }

        if(!cyl) { // found cylinder 0 ?
            floppy_motor(base, floppy_motor_off);
            return 0;
        }
    }

    tty_printf("floppy_calibrate: 10 retries exhausted\n");
    floppy_motor(base, floppy_motor_off);
    return -1;
}


int floppy_reset(int base) {

    outb(base + FLOPPY_DOR, 0x00); // disable controller
    outb(base + FLOPPY_DOR, 0x0C); // enable controller

    irq_wait(floppy_irq); // sleep until interrupt occurs

    {
        int st0, cyl; // ignore these here..
        floppy_check_interrupt(base, &st0, &cyl);
    }

    // set transfer speed 500kb/s
    outb(base + FLOPPY_CCR, 0x00);

    //  - 1st byte is: bits[7:4] = steprate, bits[3:0] = head unload time
    //  - 2nd byte is: bits[7:1] = head load time, bit[0] = no-DMA
    //
    //  steprate    = (8.0ms - entry*0.5ms)*(1MB/s / xfer_rate)
    //  head_unload = 8ms * entry * (1MB/s / xfer_rate), where entry 0 -> 16
    //  head_load   = 1ms * entry * (1MB/s / xfer_rate), where entry 0 -> 128
    //
    floppy_write_cmd(base, CMD_SPECIFY);
    floppy_write_cmd(base, 0xdf); /* steprate = 3ms, unload time = 240ms */
    floppy_write_cmd(base, 0x02); /* load time = 16ms, no-DMA = 0 */

    // it could fail...
    if(floppy_calibrate(base)) return -1;
   
}


typedef enum {
    floppy_dir_read = 1,
    floppy_dir_write = 2
} floppy_dir;


// we statically reserve a totally uncomprehensive amount of memory
// must be large enough for whatever DMA transfer we might desire
// and must not cross 64k borders so easiest thing is to align it
// to 2^N boundary at least as big as the block
#define floppy_dmalen 0x4800
static const char floppy_dmabuf[floppy_dmalen]
                  __attribute__((aligned(0x8000)));

static void floppy_dma_init(floppy_dir dir) {

    union {
        unsigned char b[4]; // 4 bytes
        unsigned long l;    // 1 long = 32-bit
    } a, c; // address and count

    a.l = (unsigned) &floppy_dmabuf;
    c.l = (unsigned) floppy_dmalen - 1; // -1 because of DMA counting

    // check that address is at most 24-bits (under 16MB)
    // check that count is at most 16-bits (DMA limit)
    // check that if we add count and address we don't get a carry
    // (DMA can't deal with such a carry, this is the 64k boundary limit)
    if((a.l >> 24) || (c.l >> 16) || (((a.l&0xffff)+c.l)>>16)) {
        tty_printf("floppy_dma_init: static buffer problem\n");
    }

    unsigned char mode;
    switch(dir) {
        // 01:0:0:01:10 = single/inc/no-auto/to-mem/chan2
        case floppy_dir_read:  mode = 0x46; break;
        // 01:0:0:10:10 = single/inc/no-auto/from-mem/chan2
        case floppy_dir_write: mode = 0x4a; break;
        default: tty_printf("floppy_dma_init: invalid direction");
                 return; // not reached, please "mode user uninitialized"
    }

    outb(0x0a, 0x06);   // mask chan 2

    outb(0x0c, 0xff);   // reset flip-flop
    outb(0x04, a.b[0]); //  - address low byte
    outb(0x04, a.b[1]); //  - address high byte

    outb(0x81, a.b[2]); // external page register

    outb(0x0c, 0xff);   // reset flip-flop
    outb(0x05, c.b[0]); //  - count low byte
    outb(0x05, c.b[1]); //  - count high byte

    outb(0x0b, mode);   // set mode (see above)

    outb(0x0a, 0x02);   // unmask chan 2
}