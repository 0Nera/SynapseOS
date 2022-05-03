/*
 * Linker script for a EOS program written in C
 */

ENTRY(main)

SECTIONS {
    . = 0x10000000;

    .text : AT(ADDR(.text) - 0x00000000) {
        *(.multiboot)
        *(.text)
        . = ALIGN(0x1000);
    }

    .rodata : AT(ADDR(.rodata) - 0x00000000) {
        *(.rodata*)
        . = ALIGN(0x1000);
    }

    .data : AT(ADDR(.data) - 0x00000000) {
        *(.data)
        . = ALIGN(0x1000);
    }

    .bss : AT(ADDR(.bss) - 0x00000000) {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
        . = ALIGN(0x1000);
    }
}
