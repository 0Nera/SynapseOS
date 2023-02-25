ENTRY(_start)

SECTIONS {
    . = 0x00200000 /*0x00200000*/;
    KERNEL_BEGIN = .;
    KERNEL_BEGIN_PHYS = . - 0x00200000;
    
    .text BLOCK(128K) : ALIGN(4K) {
        text_sect_phys_addr = .;
        *(.multiboot)
        *(.text)
    }
    .rodata BLOCK(16K) : ALIGN(4K) {
        rodata_sect_phys_addr = .;
        *(.rodata)
    }
    .data BLOCK(16K) : ALIGN(4K) {
        data_sect_phys_addr = .;
        *(.data)
    }
    .bss BLOCK(16K) : ALIGN(4K) {
        bss_sect_phys_addr = .;
        *(COMMON)
        *(.bss)
        bss_phys_end = .;
    }

	/DISCARD/ : {
		*(.note.gnu.build-id)
	}

    KERNEL_END_PHYS = . - 0x00200000;
    KERNEL_END = .; 
    KERNEL_SIZE_PHYS = KERNEL_END_PHYS - KERNEL_BEGIN_PHYS;
    KERNEL_SIZE = KERNEL_END - KERNEL_BEGIN;

}
