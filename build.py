import os, shutil, sys, tarfile, os.path


SYS_OBJ = "bin/kernel/kernel.o bin/kernel/sys/elf.o"
ARCH_OBJ = "bin/kernel/starter.o bin/kernel/interrupts.o bin/kernel/paging.o"
MEM_OBJ = "bin/kernel/mem/pmm.o bin/kernel/mem/vmm.o bin/kernel/mem/kheap.o"
DRIVERS_OBJ = "bin/kernel/drivers/vfs.o bin/kernel/drivers/ramdisk.o bin/kernel/drivers/keyboard.o bin/kernel/drivers/floppy.o"
IO_OBJ = "bin/kernel/io/tty.o bin/kernel/io/vgafnt.o bin/kernel/io/ports.o bin/kernel/io/shell.o"
INTERRUPTS_OBJ = "bin/kernel/interrupts/gdt.o bin/kernel/interrupts/idt.o bin/kernel/interrupts/tss.o bin/kernel/interrupts/syscalls.o"
LIBK_OBJ = "bin/kernel/libk/stdlib.o bin/kernel/libk/string.o"

OBJ = SYS_OBJ + " " + ARCH_OBJ + " " + MEM_OBJ + " " + DRIVERS_OBJ + " " + IO_OBJ + " " + INTERRUPTS_OBJ + " " + LIBK_OBJ


def build_all():
    print("Building kernel")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/kernel.c -o bin/kernel/kernel.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/arch/x86/starter.s -o bin/kernel/starter.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/arch/x86/interrupts.s -o bin/kernel/interrupts.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/arch/x86/paging.s -o bin/kernel/paging.o")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/mem/pmm.c -o bin/kernel/mem/pmm.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/mem/vmm.c -o bin/kernel/mem/vmm.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/mem/kheap.c -o bin/kernel/mem/kheap.o")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/drivers/vfs.c -o bin/kernel/drivers/vfs.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/drivers/ramdisk.c -o bin/kernel/drivers/ramdisk.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/drivers/keyboard.c -o bin/kernel/drivers/keyboard.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/drivers/floppy.c -o bin/kernel/drivers/floppy.o")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/io/tty.c -o bin/kernel/io/tty.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/io/vgafnt.c -o bin/kernel/io/vgafnt.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/io/ports.c -o bin/kernel/io/ports.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/io/shell.c -o bin/kernel/io/shell.o")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/interrupts/gdt.c -o bin/kernel/interrupts/gdt.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/interrupts/idt.c -o bin/kernel/interrupts/idt.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/interrupts/tss.c -o bin/kernel/interrupts/tss.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/interrupts/syscalls.c -o bin/kernel/interrupts/syscalls.o")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/libk/stdlib.c -o bin/kernel/libk/stdlib.o")
    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/libk/string.c -o bin/kernel/libk/string.o")

    os.system("i686-elf-gcc -g -ffreestanding -Wall -Wextra -w -O0 -I kernel/include/ -c kernel/src/sys/elf.c -o bin/kernel/sys/elf.o")

    os.system("i686-elf-gcc -T kernel/link.ld -nostdlib -lgcc -o isodir/boot/kernel.elf " + OBJ)


if __name__ == "__main__":
    try:
        build_all()

        if os.path.exists("ata.qcow2"):
            pass
        else:
            os.system("qemu-img create -f qcow2 -o compat=1.1 ata.qcow2 8G")
        
        if os.path.exists("fdb.img"):
            pass
        else:
            os.system("qemu-img create fdb.img 1.44M")

        os.chdir("apps/")
        os.system("python build.py")

        shutil.rmtree("../initrd/apps", ignore_errors=True)
        shutil.copytree("../bin/apps", "../initrd/apps")

        os.chdir("../initrd")
        print
        

        with tarfile.open("../isodir/boot/initrd.tar", "w") as tar:
            for i in os.listdir():
                tar.add(i)
        
        os.chdir("../")

        
        if sys.platform == "linux" or sys.platform == "linux2":
            os.system("""grub-mkrescue -o "SynapseOS.iso" isodir/ -V SynapseOS""")
        else:
            os.system("""wsl grub-mkrescue -o "SynapseOS.iso" isodir/ -V SynapseOS """)

        os.system("qemu-system-i386 -m 1024 -name SynapseOS -soundhw all -cdrom SynapseOS.iso -fdb fdb.img -hda ata.qcow2 -serial  file:Qemu.log -no-reboot")
    except Exception as E:
        print(E)