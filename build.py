import os, shutil, sys, tarfile, os.path


SYS_OBJ = "bin/kernel/kernel.o bin/kernel/elf.o bin/kernel/tss.o bin/kernel/syscalls.o bin/kernel/gdt.o bin/kernel/idt.o"
FS_OBJ = "bin/kernel/STFS.o"
ARCH_OBJ = "bin/kernel/starter.o bin/kernel/interrupts.o bin/kernel/paging.o"
MEM_OBJ = "bin/kernel/pmm.o bin/kernel/vmm.o bin/kernel/kheap.o bin/kernel/paging_c.o bin/kernel/tasks.o"
DRIVERS_OBJ = "bin/kernel/vfs.o bin/kernel/ramdisk.o bin/kernel/keyboard.o bin/kernel/pci.o bin/kernel/ata.o bin/kernel/time.o" \
    " bin/kernel/RTL8139.o bin/kernel/dhcp.o bin/kernel/udp.o bin/kernel/net_utils.o bin/kernel/ethernet.o bin/kernel/arp.o bin/kernel/ip.o"
IO_OBJ = "bin/kernel/tty.o bin/kernel/vgafnt.o bin/kernel/ports.o bin/kernel/shell.o"
LIBK_OBJ = "bin/kernel/stdlib.o bin/kernel/string.o bin/kernel/list.o"

OBJ = SYS_OBJ + " " + FS_OBJ + " " + ARCH_OBJ + " " + MEM_OBJ + " " + DRIVERS_OBJ + " " + IO_OBJ + " " + LIBK_OBJ
CC = "i686-elf-gcc -g -ffreestanding -I kernel/include/ -c"

def build_all():
    print("Building kernel")

    os.system(f"{CC} kernel/kernel.c -o bin/kernel/kernel.o")
    os.system(f"{CC} kernel/arch/x86/starter.s -o bin/kernel/starter.o")
    os.system(f"{CC} kernel/arch/x86/interrupts.s -o bin/kernel/interrupts.o")
    os.system(f"{CC} kernel/arch/x86/paging.s -o bin/kernel/paging.o")
    os.system(f"{CC} kernel/arch/x86/tasks.s -o bin/kernel/tasks.o")

    os.system(f"{CC} kernel/src/mem/pmm.c -o bin/kernel/pmm.o")
    os.system(f"{CC} kernel/src/mem/vmm.c -o bin/kernel/vmm.o")
    os.system(f"{CC} kernel/src/mem/kheap.c -o bin/kernel/kheap.o")
    os.system(f"{CC} kernel/src/mem/paging.c -o bin/kernel/paging_c.o")

    os.system(f"{CC} kernel/src/fs/STFS.c -o bin/kernel/STFS.o")

    os.system(f"{CC} kernel/src/drivers/vfs.c -o bin/kernel/vfs.o")
    os.system(f"{CC} kernel/src/drivers/ramdisk.c -o bin/kernel/ramdisk.o")
    os.system(f"{CC} kernel/src/drivers/keyboard.c -o bin/kernel/keyboard.o")
    os.system(f"{CC} kernel/src/drivers/pci.c -o bin/kernel/pci.o")
    os.system(f"{CC} kernel/src/drivers/ata.c -o bin/kernel/ata.o")
    os.system(f"{CC} kernel/src/drivers/time.c -o bin/kernel/time.o")
    os.system(f"{CC} kernel/src/drivers/network/RTL8139.c -o bin/kernel/RTL8139.o")
    os.system(f"{CC} kernel/src/drivers/network/ethernet.c -o bin/kernel/ethernet.o")
    os.system(f"{CC} kernel/src/drivers/network/net_utils.c -o bin/kernel/net_utils.o")
    os.system(f"{CC} kernel/src/drivers/network/arp.c -o bin/kernel/arp.o")
    os.system(f"{CC} kernel/src/drivers/network/ip.c -o bin/kernel/ip.o")
    os.system(f"{CC} kernel/src/drivers/network/dhcp.c -o bin/kernel/dhcp.o")
    os.system(f"{CC} kernel/src/drivers/network/udp.c -o bin/kernel/udp.o")

    os.system(f"{CC} kernel/src/io/tty.c -o bin/kernel/tty.o")
    os.system(f"{CC} kernel/src/io/vgafnt.c -o bin/kernel/vgafnt.o")
    os.system(f"{CC} kernel/src/io/ports.c -o bin/kernel/ports.o")
    os.system(f"{CC} kernel/src/io/shell.c -o bin/kernel/shell.o")

    os.system(f"{CC} kernel/src/libk/stdlib.c -o bin/kernel/stdlib.o")
    os.system(f"{CC} kernel/src/libk/string.c -o bin/kernel/string.o")
    os.system(f"{CC} kernel/src/libk/list.c   -o bin/kernel/list.o")

    os.system(f"{CC} kernel/src/sys/gdt.c -o bin/kernel/gdt.o")
    os.system(f"{CC} kernel/src/sys/idt.c -o bin/kernel/idt.o")
    os.system(f"{CC} kernel/src/sys/elf.c -o bin/kernel/elf.o")
    os.system(f"{CC} kernel/src/sys/tss.c -o bin/kernel/tss.o")
    os.system(f"{CC} kernel/src/sys/syscalls.c -o bin/kernel/syscalls.o")

    os.system("i686-elf-gcc -T kernel/link.ld -nostdlib -lgcc -o isodir/boot/kernel.elf " + OBJ)


if __name__ == "__main__":
    try:
        build_all()

        if os.path.exists("ata.vhd"):
            pass
        else:
            os.system("qemu-img create -f raw ata.vhd 32M")
        

        os.chdir("apps/")
        os.system("python build.py")

        shutil.rmtree("../initrd/apps", ignore_errors=True)
        shutil.copytree("../bin/apps", "../initrd/apps")

        os.chdir("../initrd")
        

        with tarfile.open("../isodir/boot/initrd.tar", "w") as tar:
            for i in os.listdir():
                tar.add(i)
        
        os.chdir("../")
        
        print("Creating ISO")

        if sys.platform == "linux" or sys.platform == "linux2":
            os.system("grub-mkrescue -o \"SynapseOS.iso\" isodir/ -V SynapseOS")
        else:
            os.system("ubuntu run grub-mkrescue -o \"SynapseOS.iso\" isodir/ -V SynapseOS ")

        qemu_command = "qemu-system-i386 -name SynapseOS -soundhw all -m 16" \
            " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 " \
            " -cdrom SynapseOS.iso -hda ata.vhd -serial  file:Qemu.log"
        
        os.system(
            qemu_command
            )
    except Exception as E:
        print(E)