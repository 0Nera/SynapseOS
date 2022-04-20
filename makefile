.PHONY: kernel winiso


OUTK=bin/kernel
KD=kernel
KS=$(KD)/src
x86=$(KD)/arch/x86
ISODIR=isodir/
INCLUDE_DIR=$(KD)/include/

CC=i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I $(KD)/include/

LDFLAGS=-T linker.ld -ffreestanding -nostdlib -lgcc
CCFLAGS=-g -I include -ffreestanding -Wall -Wextra -O0 -I $(KD)/include/

SYS_OBJ=$(OUTK)/kernel.o $(OUTK)/sys/elf.o 
ARCH_OBJ=$(OUTK)/starter.o $(OUTK)/interrupts.o $(OUTK)/paging.o
MEM_OBJ=$(OUTK)/mem/pmm.o $(OUTK)/mem/vmm.o $(OUTK)/mem/kheap.o
DRIVERS_OBJ=$(OUTK)/drivers/vfs.o $(OUTK)/drivers/ramdisk.o $(OUTK)/drivers/keyboard.o
IO_OBJ=$(OUTK)/io/tty.o $(OUTK)/io/vgafnt.o $(OUTK)/io/ports.o $(OUTK)/io/shell.o
INTERRUPTS_OBJ=$(OUTK)/interrupts/gdt.o $(OUTK)/interrupts/idt.o $(OUTK)/interrupts/tss.o $(OUTK)/interrupts/syscalls.o
LIBK_OBJ=$(OUTK)/libk/stdlib.o $(OUTK)/libk/string.o

OBJ=$(SYS_OBJ) $(ARCH_OBJ) $(MEM_OBJ) $(DRIVERS_OBJ) $(IO_OBJ) $(INTERRUPTS_OBJ) $(LIBK_OBJ) 



kernel: 
	$(CC) $(CFLAGS) -c $(KD)/kernel.c 				-o $(OUTK)/kernel.o
	$(CC) $(CFLAGS) -c $(x86)/starter.s 			-o $(OUTK)/starter.o
	$(CC) $(CFLAGS) -c $(x86)/interrupts.s 			-o $(OUTK)/interrupts.o
	$(CC) $(CFLAGS) -c $(x86)/paging.s 				-o $(OUTK)/paging.o

	$(CC) $(CFLAGS) -c $(KS)/mem/pmm.c 				-o $(OUTK)/mem/pmm.o
	$(CC) $(CFLAGS) -c $(KS)/mem/vmm.c 				-o $(OUTK)/mem/vmm.o
	$(CC) $(CFLAGS) -c $(KS)/mem/kheap.c 			-o $(OUTK)/mem/kheap.o

	$(CC) $(CFLAGS) -c $(KS)/drivers/vfs.c 			-o $(OUTK)/drivers/vfs.o
	$(CC) $(CFLAGS) -c $(KS)/drivers/ramdisk.c 		-o $(OUTK)/drivers/ramdisk.o
	$(CC) $(CFLAGS) -c $(KS)/drivers/keyboard.c 	-o $(OUTK)/drivers/keyboard.o

	$(CC) $(CFLAGS) -c $(KS)/io/tty.c 				-o $(OUTK)/io/tty.o
	$(CC) $(CFLAGS) -c $(KS)/io/vgafnt.c 			-o $(OUTK)/io/vgafnt.o
	$(CC) $(CFLAGS) -c $(KS)/io/ports.c 			-o $(OUTK)/io/ports.o
	$(CC) $(CFLAGS) -c $(KS)/io/shell.c 			-o $(OUTK)/io/shell.o

	$(CC) $(CFLAGS) -c $(KS)/interrupts/gdt.c 		-o $(OUTK)/interrupts/gdt.o
	$(CC) $(CFLAGS) -c $(KS)/interrupts/idt.c 		-o $(OUTK)/interrupts/idt.o
	$(CC) $(CFLAGS) -c $(KS)/interrupts/tss.c 		-o $(OUTK)/interrupts/tss.o
	$(CC) $(CFLAGS) -c $(KS)/interrupts/syscalls.c 	-o $(OUTK)/interrupts/syscalls.o

	$(CC) $(CFLAGS) -c $(KS)/libk/stdlib.c 			-o $(OUTK)/libk/stdlib.o
	$(CC) $(CFLAGS) -c $(KS)/libk/string.c 			-o $(OUTK)/libk/string.o

	$(CC) $(CFLAGS) -c $(KS)/sys/elf.c 				-o $(OUTK)/sys/elf.o

	$(CC) -nostdlib -lgcc -T link.ld -o 


winiso:
	wsl grub-mkrescue -o SynapseOS.iso $(ISODIR)

	