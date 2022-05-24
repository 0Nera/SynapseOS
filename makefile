OBJ2=$(wildcard bin/kernel*.o) 
SRC2=$(wildcard kernel/src/*.c) 

SOURCES := $(shell find .  *.c)

all:
	echo $(OBJ2) , $(SRC2) , $(filter %$*.c, $(SOURCES)) , $(SOURCES)
	# i686-elf-gcc  -o kernel.elf kernel/src/*.c 



run: 
	qemu-system-i386 -name SynapseOS -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 -cdrom SynapseOS.iso -hda ata.vhd -serial  file:Qemu.log