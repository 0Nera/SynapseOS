OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)


SECTIONS {
	. = 8M;

	.text BLOCK(4K) : ALIGN(4K) {
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
	
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}
	
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
}
