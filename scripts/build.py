#!/usr/bin/python3
# -*- coding: utf-8 -*-
import os
import time
import glob
import shutil
import argparse

SRC_TARGETS = []
BIN_TARGETS = []

ARCH = "i686" # "x86_64", "arm", "e2k"
ARCH_DIR = "x86" # "x86_64", "arm", "e2k"

DEBUG_FLAGS = f"-D DEBUG=1"

CC = f"{ARCH}-elf-gcc"  #  -march=i586
LD = f"{ARCH}-elf-gcc"  #  -march=i586

CC_GCC = f"" # f"-finput-charset=unicode -fexec-charset=unicode -finput-charset=utf8 -fexec-charset=utf8"
CC_OPTIM = f"-Wall -Wextra -O0"
CC_PROTECT = f"-mno-sse -ffreestanding -fno-stack-protector -nostdlib"
CC_FLAGS = f"{CC_PROTECT} {DEBUG_FLAGS} {CC_OPTIM} -I kernel//include// -I arch//{ARCH_DIR}//include// -c -fPIE -fPIC"

LD_FLAGS = f"{CC_GCC} {CC_PROTECT}  -T arch//{ARCH_DIR}//link.ld -nostdlib -O0 "


''' Сборка ядра '''
def build_kernel():
    print("Сборка ядра")

    files = glob.glob(f"arch//{ARCH_DIR}//**//*.s", recursive=True) + \
            glob.glob(f"arch//{ARCH_DIR}//**//*.c", recursive=True) + \
            glob.glob(f"kernel//**//*.c", recursive=True)

    for i in range(len(files)):
        SRC_TARGETS.append(files[i])
        BIN_TARGETS.append(os.path.join("bin//kernel//", os.path.basename(SRC_TARGETS[i]) + '.o '))
    
    shutil.rmtree("bin", ignore_errors=True)

    if not os.path.exists("bin//"):
        os.mkdir("bin//")

    if not os.path.exists("bin//kernel//"):
        os.mkdir("bin//kernel//")

    for i in range(0, len(SRC_TARGETS)):
        os.system(f"{CC} {DEBUG_FLAGS} {CC_FLAGS} {SRC_TARGETS[i]} -o {BIN_TARGETS[i]}")
        print(f"{CC} {CC_FLAGS} {SRC_TARGETS[i]} -o {BIN_TARGETS[i]}")
    
    BIN_TARGETS.append("bin//font_psf.o")

    print(f"{LD} -r -b binary -o bin//font_psf.o kernel/src/graf/font.psf")
    os.system(f"{LD} -r -b binary -o bin//font_psf.o kernel/src/graf/font.psf")
    
    print(f"{LD} {LD_FLAGS} -o isodir//boot//kernel.elf {' '.join(str(x) for x in BIN_TARGETS)}")
    os.system(f"{LD} {LD_FLAGS} -o isodir//boot//kernel.elf {' '.join(str(x) for x in BIN_TARGETS)}")
    
	
''' Генерация документации '''
def build_docs():
    print("Генерация документации")
    os.system("doxygen scripts//Doxyfile")


''' Сборка модулей '''
def build_modules():
    #os.system("fasm mod/seb/test.asm isodir/modules/test.seb")
    #os.system("python3 scripts/build_modules.py")
    pass


''' Сборка ISO limine '''
def build_iso_limine():
    print("Сборка ISO limine")
    start_time = time.time()
    os.system("""xorriso -as mkisofs -b limine-cd.bin \
          -no-emul-boot -boot-load-size 4 -boot-info-table \
          --efi-boot limine-cd-efi.bin \
          -efi-boot-part --efi-boot-image --protective-msdos-label \
          isodir -o SynapseOS-limine.iso""")
    
    os.system("limine-deploy SynapseOS-limine.iso")
    
    print(f"Сборка ISO//Limine образа заняла: {(time.time() - start_time):2f} секунд.")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='SynapseOS build helper')
    parser.add_argument('-compiler', help='build with your compiler')
    parser.add_argument('-linker', help='build with your linker')
    args = parser.parse_args()

    if args.compiler != None:
        CC = args.compiler

    if args.linker != None:
        CC = args.linker

    start_time = time.time()
    build_kernel()
    print(f"Сборка ядра заняла: {(time.time() - start_time):2f} секунд.")

    start_time = time.time()
    build_modules()
    print(f"Сборка модулей заняла: {(time.time() - start_time):2f} секунд.")
    
    build_iso_limine()

    #build_docs()

    if ARCH == "i686":
        QEMU_DEV = f"-device rtl8139,id=nic0"
        QEMU = f"qemu-system-i386 -m 128 -d guest_errors -no-reboot {QEMU_DEV} " # -cpu pentium3
        
        os.system(f"{QEMU} -monitor stdio -cdrom SynapseOS-limine.iso -serial file:serial.log")
