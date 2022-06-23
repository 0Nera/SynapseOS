#include <kernel.h>

// Defines a GDT entry
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// Special pointer which includes the limit: The max bytes
// taken up by the GDT, minus 1
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Our GDT, with 6 entries, and finally our special GDT pointer
struct gdt_entry gdt[GDT_MAX_DESCRIPTORS];
struct gdt_ptr gp;

// Function arch/i386/gdt.S, loads GDT from the pointeer of a gdt_ptr
extern void gdt_flush(struct gdt_ptr *gdt_ptr_addr);

// Setup a descriptor in the Global Descriptor Table
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran) {
    // Setup the descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Should be called by the kernal on initializaiton. This will setup the
// special GDT pointer, set up the first 3 entries in our GDT, and then
// finally call gdt_flush() in our assembler file in order to tell the
// processor where the new GDT is and update the new segment registers
void gdt_init() {
    // Setup the GDT pointer and limit
    gp.limit = (sizeof(struct gdt_entry) * GDT_MAX_DESCRIPTORS) - 1;
    gp.base = (uint32_t) &gdt;

    // Our NULL descriptor, required
    gdt_set_gate(0, 0, 0, 0, 0);
    /* Kernel code, access(9A = 1 00 1 1 0 1 0)
        1   present
        00  ring 0
        1   always 1
        1   code segment
        0   can be executed by ring lower or equal to DPL,
        1   code segment is readable
        0   access bit, always 0, cpu set this to 1 when accessing this sector
    */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
    /* Kernel data, access(92 = 1 00 1 0 0 1 0)
        Only differ at the fifth bit(counting from least insignificant bit), 0 means it's a data segment.
    */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
    // User code and data segments respectively, only differ from pervious in ring number(ring 3)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // added 27.02.20
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // added 27.02.20
    // Flush out the old GDT and install the new changes!
    gdt_flush(&gp);

    // Initialize TSS
    tss_init(5, 0x10, 0);
    tss_flush();
}
