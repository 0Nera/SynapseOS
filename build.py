import os, shutil, sys, tarfile, time, glob
from reprlib import recursive_repr

MEMORY = "64M"

_CC = "clang -target i386-pc-none-elf"
LD = "ld.lld"
_CC = "i686-elf-gcc -lgcc -W"
LD = _CC
CFLAGS = " -nostdlib -mno-sse -mno-avx -ggdb -O0 -ffreestanding -I kernel/include/ -c"
CC = f"{_CC} {CFLAGS}"

SRC_TARGETS = []
BIN_TARGETS = []
BINFLDR = "bin\\" if not (sys.platform == "linux" or sys.platform == "linux2") else "bin/"

def warn(message):
    print(f"[\x1b[33;1mWARNING\x1b[0m]: {message}")

def compile(binary, source, cur="--", total="--", warnings=False):
    print(f"[\x1b[32;1mBUILD\x1b[0m] [{cur}/{total}]: Compiling: {source}")
    os.system(f"{CC}  {'' if warnings else ' -w '}  -o ./{binary} {source}")

def compile_kernel(warnings=False):
    print("Compiling...")
    if not os.path.isdir("bin"):
        os.mkdir("bin")
    if not os.path.isdir(f"bin{os.sep}kernel"):
        os.mkdir(f"bin{os.sep}kernel")
    
    filescount = len(SRC_TARGETS)

    updated = []
    for i in range(filescount):
        BIN_TARGETS.append(os.path.join(BINFLDR, os.path.basename(SRC_TARGETS[i]) + '.o '  ))
        srcf = SRC_TARGETS[i]
        objf = os.path.join(BINFLDR, os.path.basename(SRC_TARGETS[i]) + '.o')
        if os.path.isfile(objf):
            if os.path.getmtime(srcf)>os.path.getmtime(objf):
                updated.append(srcf)
        else:
            updated.append(srcf)

    filescount = len(updated)

    for i in range(filescount):
        compile(os.path.join("bin\\" if not (sys.platform == "linux" or sys.platform == "linux2") else "bin/", os.path.basename(updated[i]) + '.o '), updated[i], i+1, filescount, warnings)

def link_kernel():
    print("Linking...")
    # print(f"BIN_TARGETS = {BIN_TARGETS}")
    os.system(f"{LD} -T kernel/link.ld -nostdlib -o isodir/boot/kernel.elf " + ''.join(BIN_TARGETS))

def build_kernel(warnings=False):
    print("Building kernel at ", os.getcwd())
    start_time = time.time()

    files = glob.glob("kernel/**/*.c", recursive=True) + glob.glob("kernel/**/*.s", recursive=True)

    for i in files:
        SRC_TARGETS.append(i)
    
    compile_kernel(warnings)
    link_kernel()
    x = 0
    while not os.path.exists("isodir/boot/kernel.elf"):
        x += 1
        compile_kernel(warnings)
        link_kernel()
        print(f"Errors: {x}")
        if x > 4:
            exit(-1)
    print(f"Сборка ядра заняла: {(time.time() - start_time):2f} сек.")


def build_apps():
    os.chdir("apps/")
    os.system("python3 build.py")

    shutil.rmtree("../initrd/apps", ignore_errors=True)
    shutil.copytree("../bin/apps", "../initrd/apps")
    shutil.rmtree("../initrd/res", ignore_errors=True)
    shutil.copytree("res", "../initrd/res")

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
    
    print(f"Сборка ISO/Grub образа заняла: {(time.time() - start_time):2f} сек.")


def create_iso_l():
    print("Creating ISO with limine")
    start_time = time.time()

    os.system("git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1")
    os.system("make -C limine")
    os.system("mkdir -p iso_root")
    os.system("""cp -v isodir/boot/kernel.elf isodir/boot/initrd.tar limine.cfg limine/limine.sys \
        limine/limine-cd.bin limine/limine-cd-efi.bin isodir/boot/bg.bmp iso_root/
    """)
    os.system("""xorriso -as mkisofs -b limine-cd.bin \
          -no-emul-boot -boot-load-size 4 -boot-info-table \
          --efi-boot limine-cd-efi.bin \
          -efi-boot-part --efi-boot-image --protective-msdos-label \
          iso_root -o SynapseOS-limine.iso""")
    os.system("./limine/limine-deploy SynapseOS-limine.iso")
    
    print(f"Сборка ISO/Limine образа заняла: {(time.time() - start_time):2f} сек.")


def run_qemu():
    if os.path.exists("ata.vhd"):
        pass
    else:
        os.system("qemu-img create -f raw ata.vhd 32M")
    os.system("qemu-img create -f raw fdb.img 1440K")
    
    qemu_command = f"qemu-system-i386 -name SynapseOS -soundhw pcspk -m {MEMORY}" \
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 " \
        " -cdrom SynapseOS.iso -fdb fdb.img -hda ata.vhd -serial  file:Qemu.log -d guest_errors -rtc base=localtime"
        
    os.system(qemu_command)

def run_kvm():
    " Это помогает запускать SynapseOS быстрее, по сравнению с обычным режимом"
    " Paimon is not emergency food... "
    if not os.path.exists("ata.vhd"):
        os.system("qemu-img create -f raw ata.vhd 32M")
    
    qemu_command = f"qemu-system-i386 -name SynapseOS -soundhw pcspk -m {MEMORY}" \
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 " \
        " -cdrom SynapseOS.iso -hda ata.vhd -serial  file:Qemu.log -accel kvm -d guest_errors -rtc base=localtime"
        
    os.system(qemu_command)


def run_qemu_debug():
    if os.path.exists("ata.vhd"):
        pass
    else:
        os.system("qemu-img create -f raw ata.vhd 32M")

    
    qemu_command = f"qemu-system-i386 -name SynapseOS -soundhw pcspk -m {MEMORY}" \
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 " \
        " -d guest_errors -cdrom SynapseOS.iso -hda ata.vhd -serial  file:Qemu.log -rtc base=localtime" 
    print("gdb kernel.elf -ex \"target remote localhost:1234\"")
    os.system(
        qemu_command + """ -s -S"""
        )

if __name__ == "__main__":
    try:
        start_time = time.time()

        # Стандартная сборка

        warnings = False
        args = sys.argv[1:] # Filter out program name (build.py)

        i = 0
        while i<len(args):
            elem = args[i]
            if elem.startswith("--warn"): # This may be --warn or --warni or --warning and so on
                warn("Вывод предупреждений компилятора включен")
                warnings = True
                del args[i]
                continue
            i+=1

        if warnings: CFLAGS = CFLAGS[2:]; CC = f"{_CC} {CFLAGS}";

        if not args:
            build_kernel(warnings)
            build_apps()
            create_iso()
            print(f"Время сборки: {(time.time()-start_time):2f} сек.")
            run_qemu()
        else:
            for i in range(1, len(sys.argv)):
                if sys.argv[i] == "kernel":
                    build_kernel()
                elif sys.argv[i] == "apps":
                    build_apps()
                elif sys.argv[i] == "isol":
                    create_iso_l()
                elif sys.argv[i] == "iso":
                    create_iso()
                elif sys.argv[i] == "run":
                    run_qemu()
                elif sys.argv[i] == "runk":
                    run_kvm()
                elif sys.argv[i] == "rund":
                    run_qemu_debug()
                else:
                    print(f"Ошибка, неизвестный аргумент: {sys.argv[i]}")
        print(f"Конец: {time.time() - start_time}")

    except Exception as E:
        print(E)
