OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)


SECTIONS {
	/* 
		ELF с релокацией в Америку? 
		Нет, релокация только в элитные 128 мегабайт(Армению)
	*/
	/*. = 128M;*/
	. = 1M;

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
