OUTPUT_FORMAT(elf32-i386)

ENTRY(_loader)

SECTIONS {
    /* The kernel will live at 3GB + 1MB in the virtual
     * address space, which will be mapped to 1MB in the
     * physical address space
     */
    . = 0xC0100000;

    kernel_virt_start = .;
    kernel_phys_start = . - 0xC0000000;

    .text : AT(ADDR(.text) - 0xC0000000) {
        *(.multiboot)
        *(.text)
        . = ALIGN(0x1000);
    }

    .rodata : AT(ADDR(.rodata) - 0xC0000000) {
        *(.rodata*)
        . = ALIGN(0x1000);
    }

    .data : AT(ADDR(.data) - 0xC0000000) {
        *(.data)
        . = ALIGN(0x1000);
    }

    .bss : AT(ADDR(.bss) - 0xC0000000) {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
        . = ALIGN(0x1000);
    }

    kernel_virt_end = .;
    kernel_phys_end = . - 0xC0000000;
}
