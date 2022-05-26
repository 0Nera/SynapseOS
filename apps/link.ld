ENTRY(main)

SECTIONS {
    . = 0x10000000;

    .text : AT(ADDR(.text) - 0x00000000) {
        *(.text)
        . = ALIGN(0x1000);
    }

    .data : AT(ADDR(.data) - 0x00000000) {
        *(.data)
        . = ALIGN(0x1000);
    }

}
