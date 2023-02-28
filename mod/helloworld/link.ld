OUTPUT_FORMAT("elf32-i386")
ENTRY(module_init)


SECTIONS {
	/* 
		ELF с релокацией в Америку? 
		Нет, релокация только в элитные 128 мегабайт(Армению)
	*/

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
 
  /DISCARD/ : {
    *(.drectve)
    *(.edata)
    *(.eh_frame)
  }
}
