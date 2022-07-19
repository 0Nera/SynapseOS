import os, shutil, sys, tarfile, time, glob


def listdir_fullpath(d):
    with os.scandir(d) as it:
        for entry in it:
            if not entry.name.startswith('.') and entry.is_file():
                print(entry.name)

def build_kernel():
    print("Building kernel", os.getcwd(), os.listdir())
    CC = "clang -target i386-pc-none-elf -w -mno-sse -mno-avx -O0 -ffreestanding -I kernel/include/ -c"
    start_time = time.time()
    
    SRC_TARGETS = []
    BIN_TARGETS = []

    files = glob.glob("kernel/**/*.c", recursive=True) + glob.glob("kernel/**/*.s", recursive=True)
    
    for i in files:
        SRC_TARGETS.append(i)
        BIN_TARGETS.append(os.path.join(".\\bin\\", os.path.basename(i) + '.o '  ))
        print(i)

    shutil.rmtree(".\\bin\kernel\\", ignore_errors=True)
    os.mkdir(".\\bin\kernel\\")
    
    for i in range(0, len(SRC_TARGETS)):
        os.system(f"{CC} {SRC_TARGETS[i]} -o {BIN_TARGETS[i]}")
        

    print(f"BIN_TARGETS = {BIN_TARGETS}")
    os.system("ld.lld -T kernel/link.ld -nostdlib -o isodir/boot/kernel.elf " + ''.join(BIN_TARGETS))
    print(f"Build end at: {time.time() - start_time}")



def build_apps():
    os.chdir("apps/")
    os.system("python build.py")

    shutil.rmtree("../initrd/apps", ignore_errors=True)
    shutil.copytree("../bin/apps", "../initrd/apps")

    os.chdir("../initrd")
        

    with tarfile.open("../isodir/boot/initrd.tar", "w") as tar:
        for i in os.listdir():
            tar.add(i)
    os.chdir("../")


def create_iso():
    print("Creating ISO")
    start_time = time.time()

    if sys.platform == "linux" or sys.platform == "linux2":
        os.system("grub-mkrescue -o \"SynapseOS.iso\" isodir/ -V SynapseOS")
    else:
        os.system("ubuntu run grub-mkrescue -o \"SynapseOS.iso\" isodir/ -V SynapseOS ")
    
    print(f"Build end at: {time.time() - start_time}")


def run_qemu():
    if os.path.exists("ata.vhd"):
        pass
    else:
        os.system("qemu-img create -f raw ata.vhd 32M")
    
    qemu_command = "qemu-system-i386 -name SynapseOS -soundhw pcspk -m 16" \
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 " \
        " -cdrom SynapseOS.iso -hda ata.vhd -serial  file:Qemu.log"
        
    os.system(
        qemu_command
        )


def run_qemu_debug():
    if os.path.exists("ata.vhd"):
        pass
    else:
        os.system("qemu-img create -f raw ata.vhd 32M")
    
    qemu_command = "qemu-system-i386 -name SynapseOS -soundhw pcspk -m 16" \
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 " \
        " -cdrom SynapseOS.iso -hda ata.vhd -serial  file:Qemu.log" 
    print("gdb kernel.elf -ex target remote localhost:1234")
    os.system(
        qemu_command + """ -s -S"""
        )


if __name__ == "__main__":
    try:
        start_time = time.time()

        # Стандартная сборка
        
        if len(sys.argv) == 1:
            build_kernel()
            build_apps()
            create_iso()
            run_qemu()
        else:
            for i in range(1, len(sys.argv)):
                if sys.argv[i] == "kernel":
                    build_kernel()
                elif sys.argv[i] == "apps":
                    build_apps()
                elif sys.argv[i] == "iso":
                    create_iso()
                elif sys.argv[i] == "run":
                    run_qemu()
                elif sys.argv[i] == "rund":
                    run_qemu_debug()
                else:
                    print(f"Ошибка, неизвестный аргумент: {sys.argv[i]}")
        print(f"Конец: {time.time() - start_time}")

    except Exception as E:
        print(E)