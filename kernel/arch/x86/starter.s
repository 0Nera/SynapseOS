# Константы мультизагрузки для GRUB
.set ALIGN,		 1<<0				                  # Выравнивание загруженных модулей по границам страницы
.set MEMINFO,	 1<<1				                  # Просим предоставить карту памяти
.set VBE_MODE, 1<<2                         # VBE mode flag. GRUB will set it for us and provide info about it.
.set FLAGS,		 ALIGN | MEMINFO	| VBE_MODE	# Поле "флага" мультизагрузки
.set MAGIC,		 0x1BADB002			              # Магическое число, чтобы загрузчик нашел этот заголовок
.set CHECKSUM, -(MAGIC + FLAGS)	            # Контрольная сумма вышеперечисленного

# Объявляем мультизагрузочный заголовок, который помечает программу как ядро.
# Это магические значения, которые задокументированы в стандарте мультизагрузки.
# Загрузчик будет искать этот заголовок в первых 8 килобайтах файла ядра, выровненного по 32-битной границе.
# Подпись находится в отдельном разделе, поэтому заголовок можно принудительно разместить в первых 8 килобайтах файла ядра.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM          # Контрольная сумма
.long 0, 0, 0, 0, 0     # Неиспользуется
.long 0                 # 0 - графический режим
.long 1280, 720, 32     # Ширина, длина, глубина


# Стандарт мультизагрузки не определяет значение указателя стека зарегистрируйте esp, и ядро должно предоставить стек. 
# Следующий код выделяют место для небольшой стопки, создавая символ внизу, затем выделит для него 16 килобайт и поставит символ вверху. 
# Стек будет расти вниз. Это, видимо, в своем собственном разделе, так что это можно пометить, но на самом деле там ничего нет.
# Теоретически это позволяет нам избежать 16 килобайт мусора в двоичном файле.
# Стек должен быть 16-байтным, выровненным для x86 в соответствии с системным стандартом V ABI, поэтому мы делаем это здесь.

# Выделение стека для главного потока
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
    .skip 65536 # 64 килобайта на стек            
stack_top:

# Базовый адрес ядра
.set KERNEL_VIRTUAL_BASE, 0xC0000000                  # 3 гигабайта
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22)  # 

# Declares the boot Paging directory to load a virtual higher half kernel
.section .data
.align 0x1000
.global _boot_page_directory
_boot_page_directory:
# using 4MB pages
# identity map virt 0x00000000-0xc0000000
.set i, 0x00000083
.rept 0x300 # 0x300 = 0xbfc00000/0x00400000 + 1
    .long i
    .set i, i+0x00400000
.endr

# map virt 0xc0000000-0xc0400000 to phys 0x00000000-0x00400000
.long 0x00000083

# identity map virt 0xc0400000-4G
.set i, 0xc0400083
.rept 0xFF # 0xFF = (0xffc00000 - 0xc0400000)/0x00400000 + 1
    .long i
    .set i, i+0x00400000
.endr


# Text section
.section .text
.global _loader
_loader:
  mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %ecx
  mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %edx
  or $0x00000003, %ecx
  mov %ecx, 0xFFC(%edx) #bpd + 4092 i.e we write address of page_dir|0000003 to last pde

  # Load Page Directory Base Register. Until paging is set up, the code must
  # be position-independent and use physical addresses, not virtual ones
  mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %ecx
  mov %ecx, %cr3

  # Set PSE bit in CR4 to enable 4MB pages.
  mov %cr4, %ecx
  or $0x00000010, %ecx
  mov %ecx, %cr4

  # Set PG bit in CR0 to enable paging.
  mov %cr0, %ecx
  or $0x80000000, %ecx
  mov %ecx, %cr0

  movl $_start, %edx
  jmp *%edx

# The Higher-Half Kernel entry point.
.global _start
.type _start, @function
_start:
	# Unmap the identity-mapped first 4MB of physical address space.
  	# movl $0, (_boot_page_directory) #uncommenting this crashes all, figure why???
  	# invlpg (0)

  	# Enter protection mode
  	mov %cr0, %eax
  	or $1, %al
  	mov %eax, %cr0

	# set up the stack
	mov $stack_top, %esp

	# This is where we would set up the GDT if we were setting one
	# up -- but there's nothing here for now.

	push %ebx # Multiboot structure
	push %eax # Multiboot magic number

	# Вызов ядра из C  кода

	call kernel

_hlt_loop:
  jmp _hlt_loop

	# На всякий случай повторно вешаем ядро
	cli
1:	hlt
	
	# Еще один цикл для надежности
	jmp 1b

# Мы устанавливаем размер метки _start — что предположительно означает, что это какой-то объект с атрибутами, а не просто метка — на разницу между нашей текущей точкой и положением символа.
.size _start, . - _start
