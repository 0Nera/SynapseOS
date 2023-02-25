ENTRY(_start)

SECTIONS {
    . = 2M;
    KERNEL_BEGIN = . + 0xC0000000;
    KERNEL_BEGIN_PHYS = .;
    
    .multiboot : {
        text_sect_phys_addr = .;
        *(.multiboot)
    }
    .preinit : {
        *(.preinit)
    }
    . += 0xC0000000;
    .text ALIGN(4K) : AT (ADDR(.text) - 0xC0000000) {
        *(.text)
    }
    .rodata ALIGN(4K) : AT (ADDR(.rodata) - 0xC0000000) {
        rodata_sect_phys_addr = .;
        *(.rodata)
    }
    .data ALIGN(4K) : AT (ADDR(.data) - 0xC0000000) {
        data_sect_phys_addr = .;
        *(.data)
    }
    .bss ALIGN(4K) : AT (ADDR(.bss) - 0xC0000000) {
        bss_sect_phys_addr = .;
        *(COMMON)
        *(.bss)
        bss_phys_end = .;
    }

	/DISCARD/ : {
		*(.note.gnu.build-id)
	}

    KERNEL_END_PHYS = . - 0xC0000000;
    KERNEL_END = .; 
    KERNEL_SIZE_PHYS = KERNEL_END_PHYS - KERNEL_BEGIN_PHYS;
    KERNEL_SIZE = KERNEL_END - KERNEL_BEGIN;

}
