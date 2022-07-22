"""
    Build script for SynapseOS.
    Source code: https://github.com/0Nera/SynapseOS
"""

import time

import os
import shutil
import sys
import tarfile
import glob


_CC = "clang -target i386-pc-none-elf"
LD = "ld.lld"
CFLAGS = "-w -mno-sse -mno-avx -O0 -ffreestanding -I kernel/include/ -c"

CC = f"{_CC} {CFLAGS}"

SRC_TARGETS = []
BIN_TARGETS = []


def compile_kernel():
    print("Compiling...")
    if not _os_is_linux():
        shutil.rmtree(".\\bin\\kernel\\", ignore_errors=True)
        os.mkdir("bin")
        os.mkdir("bin\\kernel")
    else:
        shutil.rmtree("./bin/kernel/", ignore_errors=True)
        if not (os.path.isdir("bin")):
            os.mkdir("bin")
        os.mkdir("bin/kernel")

    filescount = len(SRC_TARGETS)
    # TODO: Multithreading

    for i in range(filescount):
        # start_time = time.time()
        BIN_TARGETS.append(
            os.path.join("bin\\", os.path.basename(SRC_TARGETS[i]) + ".o ")
        )
        # os.system(f"echo {CC} -o {BIN_TARGETS[i]} {SRC_TARGETS[i]} & {CC} -o ./{BIN_TARGETS[i]} {SRC_TARGETS[i]} ")
        # print(f"[\x1b[32mBUILD\x1b[0m]~[{i}/{filescount}]: Compiling: {SRC_TARGETS[i]}")
        # subprocess.call(f"{CC} -o ./{BIN_TARGETS[i]} {SRC_TARGETS[i]}", shell=True, stdout=subprocess.STDOUT, stderr=subprocess.STDOUT)
        compile_step(BIN_TARGETS[i], SRC_TARGETS[i], i, filescount)

    """
    JOBS = 8 # Количество ядер используемых при сборке

    currentfileindex = 0
    while True:
        if threading.active_count()<=4:    
            if currentfileindex<filescount: 
                BIN_TARGETS.append(os.path.join("bin\\", os.path.basename(SRC_TARGETS[currentfileindex]) + '.o '  ))
                proc = threading.Thread(target=compile, args=(BIN_TARGETS[currentfileindex], 
                                                              SRC_TARGETS[currentfileindex],
                                                              currentfileindex,
                                                              filescount-1))
                proc.start()
                currentfileindex+=1
            else: break
    """


def cmd_build_kernel(warnings: bool = False):
    """`kernel` command"""
    print("Building kernel", os.getcwd(), os.listdir())
    start_time = time.time()

    files = glob.glob("kernel/**/*.c", recursive=True) + glob.glob(
        "kernel/**/*.s", recursive=True
    )

    for i in files:
        SRC_TARGETS.append(i)

    compile_kernel()
    link_kernel()

    # Weird stuff.
    x = 0
    while not os.path.exists("isodir/boot/kernel.elf"):
        x += 1
        compile_kernel()
        link_kernel()
        print(f"Errors: {x}")

    # Debug elapsed time.
    elapsed_time = time.time() - start_time
    print(f"Сборка ядра заняла: {elapsed_time:2f} сек.")


def cmd_build_apps():
    """`apps` command."""
    os.chdir("apps/")
    os.system("python build.py")

    shutil.rmtree("../initrd/apps", ignore_errors=True)
    shutil.copytree("../bin/apps", "../initrd/apps")

    os.chdir("../initrd")

    with tarfile.open("../isodir/boot/initrd.tar", "w") as tar:
        for i in os.listdir():
            tar.add(i)
    os.chdir("../")


def cmd_create_iso_grub():
    """`iso` command."""
    print("Creating ISO with grub...")
    start_time = time.time()

    if _os_is_linux():
        os.system("grub-mkrescue -o 'SynapseOS.iso' isodir/ -V SynapseOS")
    else:
        os.system("ubuntu run grub-mkrescue -o 'SynapseOS.iso' isodir/ -V SynapseOS")

    # Debug elapsed time.
    elapsed_time = time.time() - start_time
    print(f"Сборка ISO/Grub образа заняла: {elapsed_time:2f} сек.")


def cmd_create_iso_limine():
    """`isol` command."""
    print("Creating ISO with limine...")
    start_time = time.time()

    # Not documented.
    os.system(
        "git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1"
    )
    os.system("make -C limine")
    os.system("mkdir -p iso_root")
    os.system(
        """cp -v isodir/boot/kernel.elf isodir/boot/initrd.tar limine.cfg limine/limine.sys \
        limine/limine-cd.bin limine/limine-cd-efi.bin isodir/boot/bg.bmp iso_root/
    """
    )
    os.system(
        """xorriso -as mkisofs -b limine-cd.bin \
          -no-emul-boot -boot-load-size 4 -boot-info-table \
          --efi-boot limine-cd-efi.bin \
          -efi-boot-part --efi-boot-image --protective-msdos-label \
          iso_root -o SynapseOS-limine.iso"""
    )
    os.system("./limine/limine-deploy SynapseOS-limine.iso")

    # Debug elapsed time.
    elapsed_time = time.time() - start_time
    print(f"Сборка ISO/Limine образа заняла: {elapsed_time:2f} сек.")


def compile_step(binary, source, current="--", total="--"):
    """Compiles with CC, with showing current/total counter."""
    print(f"[\x1b[32;1mBUILD\x1b[0m]~[{current}/{total}]: Compiling: {source}")
    _cc_compile(source=source, output=binary)


def link_kernel():
    """Links kernel with LD. """
    print("Linking...")
    print(f"BIN_TARGETS = {BIN_TARGETS}")

    bin_targets = "".join(BIN_TARGETS)
    _ld_exec(f"-T kernel/link.ld -nostdlib -o isodir/boot/kernel.elf {bin_targets}")


def warn(message: str):
    """Show a warning colored message."""
    print(f"[\x1b[33;1mWARNING\x1b[0m]: {message}")


def cmd_run_qemu(name: str = "SynapseOS", iso: str = "SynapseOS.iso", size: int = 32):
    """`run` command."""
    _qemu_create_ata_vhd()
    _qemu_system_exec(
        f"-name {name} -soundhw pcspk -m {size}"
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 "
        f" -cdrom {iso} -hda ata.vhd -serial file:Qemu.log"
    )


def cmd_run_qemu_kvm(name: str = "SynapseOS", iso: str = "SynapseOS.iso", size: int = 32):
    """
    `runk` command. 
    Это помогает запускать SynapseOS быстрее, по сравнению с обычным режимом
    """
    _qemu_create_ata_vhd()
    _qemu_system_exec(
        f"-name {name} -soundhw pcspk -m {size}"
        " -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 "
        f" -cdrom {iso} -hda ata.vhd -serial file:Qemu.log -accel kvm"
    )


def cmd_run_qemu_debug(name: str = "SynapseOS", iso: str = "SynapseOS.iso", size: int = 32):
    """`rund` command."""
    _qemu_create_ata_vhd()
    print("gdb kernel.elf -ex target remote localhost:1234")
    _qemu_system_exec(
        f"-name {name} -soundhw pcspk -m {size} "
        "-netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 "
        f"-cdrom {iso} -hda ata.vhd -serial file:Qemu.log -s -S"
    )


def _os_is_linux() -> bool:
    """Returns true if current OS is Linux."""
    return sys.platform.startswith("linux")


def _ld_exec(cmd: str) -> int:
    """Executes LD linker command. """
    return os.system(
        f"{LD} {cmd}"
    )


def _cc_exec(cmd: str) -> int:
    """Executes CC compiler command. """
    return os.system(
        f"{CC} {cmd}"
    ) 


def _cc_compile(source: str, output: str) -> int:
    """Compiles with CC compiler."""
    return _cc_exec(f"-o ./{output} {source}")


def _qemu_system_exec(command: str, system: str = "i386") -> int:
    """Executes system command for QEMU system."""
    return os.system(f"qemu-system-{system} {command}")


def _qemu_create_ata_vhd(size: int = 32) -> int:
    """Creates `ata.vhd` file by QEMU if it is not exists."""
    if os.path.exists("ata.vhd"):
        return -1
    return os.system(f"qemu-img create -f raw ata.vhd {size}M")


def _execute_arg_cmd(arg: str):
    """Executes command by argument from terminal."""
    if arg == "kernel":
        cmd_build_kernel()
    elif arg == "apps":
        cmd_build_apps()
    elif arg == "isol":
        cmd_create_iso_limine()
    elif arg == "iso":
        cmd_create_iso_grub()
    elif arg == "run":
        cmd_run_qemu()
    elif arg == "runk":
        cmd_run_qemu_kvm()
    elif arg== "rund":
        cmd_run_qemu_debug()
    else:
        print(f"Ошибка, неизвестный аргумент: {arg}")
    

def _execute_default_build():
    """Executes default build command, when there is no special argument passed. """

    # Build.
    cmd_build_kernel(warnings)
    cmd_build_apps()

    # Create final ISO image.
    cmd_create_iso_grub()

    # Debug elapsed time.
    elapsed_time = time.time() - start_time
    print(f"Время сборки: {elapsed_time:2f} сек.")

    # Run QEMU.
    cmd_run_qemu()


if __name__ == "__main__":
    # Entry point for the build program.
    start_time = time.time()
    try:
        args = sys.argv[1:]  # Filter out program name (build.py)
        
        # Search for `--warn{n}` argument,
        # and then enable warnings if it passed.
        warnings = False
        for i in range(0, len(args)):
            argument = args[i]
            if argument.startswith(
                "--warn"
            ):  # This may be --warn or --warni or --warning and so on
                warn("Вывод предупреждений компилятора включен.")
                warnings = True
                del args[i]
                continue

        if warnings:
            CFLAGS = CFLAGS[2:]
            CC = f"{_CC} {CFLAGS}"

        # If no arguments passed, run default build,
        # if any arguments, execute by argument.
        if not args:
            _execute_default_build()
        else:
            for arg in args:
                _execute_arg_cmd(arg)
    
        # Debug elapsed time.
        elapsed_time = time.time() - start_time
        print(f"Конец. Затраченное время: {elapsed_time:2f} сек.")
    except Exception as e:
        print(f"Произошло исключение! Исключение: \n{e}")
