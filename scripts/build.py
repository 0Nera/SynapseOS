#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Сборщик ядра SynapseOS

(c) Арен Елчинян
"""

# NOTE: Для компиляторов Clang: (где 13 - это версия, её можно изменить)
# python3 scripts/build.py -compiler 'clang-13 -m32' -linker 'ld.lld-13 -m elf_i386'

import os
import re
import sys
import time
import glob
import json
import shutil
import argparse
import shutil
import importlib.util as impuitil


SRC_TARGETS = []
BIN_TARGETS = []

ARCH = "i686" # "x86_64", "arm", "e2k"
ARCH_DIR = "x86" # "x86_64", "arm", "e2k"

DEBUG_FLAGS = "-D DEBUG=1"

CC = f"{ARCH}-elf-gcc"  #  -march=i586
LD = f"{ARCH}-elf-ld"  #  -march=i586
LIMINE_DEPLOY = "limine-deploy"

CC_GCC = f"" # f"-finput-charset=unicode -fexec-charset=unicode -finput-charset=utf8 -fexec-charset=utf8"
CC_OPTIM = "-Wall -Wextra -O0"
CC_PROTECT = "-mno-sse -ffreestanding -fno-stack-protector -nostdlib"
CC_FLAGS = f"{CC_PROTECT} {DEBUG_FLAGS} {CC_OPTIM} -I kernel//include// -I arch//{ARCH_DIR}//include// -c -fPIE -fPIC"

LD_FLAGS = f"{CC_GCC} -T arch//{ARCH_DIR}//link.ld -nostdlib -O0 "

CLANGD_PATH = None
CLANGD_COMMANDS = []


def exec_cmd(cmd):
    print(cmd)
    if os.system(cmd) != 0:
        sys.exit(1)
    if CLANGD_PATH and cmd.startswith(CC):
        regexp = re.compile(r'\.(c|cpp)$')
        for entry in cmd.split(' '):
            if regexp.search(entry):
                print(entry)
                CLANGD_COMMANDS.append({
                    'directory': os.path.abspath('.'),
                    'command': cmd,
                    'file': entry
                })


''' Сборка ядра '''
def build_kernel():
    print("Сборка ядра")

    files = glob.glob(f"arch//{ARCH_DIR}//**//*.s", recursive=True) + \
            glob.glob(f"arch//{ARCH_DIR}//**//*.c", recursive=True) + \
            glob.glob("kernel//**//*.c", recursive=True)

    for i in range(len(files)):
        SRC_TARGETS.append(files[i])
        BIN_TARGETS.append(os.path.join("bin//kernel//", os.path.basename(SRC_TARGETS[i]) + '.o '))
    
    shutil.rmtree("bin", ignore_errors=True)

    if not os.path.exists("isodir/modules/"):
        os.mkdir("isodir/modules/")

    if not os.path.exists("bin/"):
        os.mkdir("bin/")

    if not os.path.exists("bin/kernel/"):
        os.mkdir("bin/kernel/")

    for i in range(0, len(SRC_TARGETS)):
        exec_cmd(f"{CC} {DEBUG_FLAGS} {CC_FLAGS} {SRC_TARGETS[i]} -o {BIN_TARGETS[i]}")
    
    BIN_TARGETS.append("bin//font_psf.o")

    exec_cmd(f"{LD} -r -b binary -o bin//font_psf.o kernel/src/graf/font.psf")
    exec_cmd(f"{CC} {LD_FLAGS} -o isodir//boot//kernel.elf {' '.join(str(x) for x in BIN_TARGETS)}")
    
	
''' Генерация документации '''
def build_docs():
    print("Генерация документации")
    os.system("doxygen scripts//Doxyfile")


''' Сборка модулей '''
def build_modules():
    #os.system("fasm mod/seb/test.asm isodir/modules/test.seb")
    #os.system("python3 scripts/build_modules.py")
    # MOD_FLAGS = "-ffreestanding -m32 -nostdlib -nostartfiles -fno-builtin -fno-stack-protector -e main"
    # os.system(f"{CC} {MOD_FLAGS} -c mod/simple/main.c -o bin/simple.o")
    # os.system(f"{CC} {MOD_FLAGS} -o isodir/modules/simple.elf bin/simple.o")
    
    # print(f"{ARCH}-elf-readelf -hls isodir/modules/simple.elf>app.elf.txt")
    # os.system(f"{ARCH}-elf-readelf -hls isodir/modules/simple.elf>app.elf.txt")
    spec = impuitil.spec_from_file_location('build', 'mod/build.py')
    mod = impuitil.module_from_spec(spec)
    sys.modules['build'] = mod
    spec.loader.exec_module(mod)
    CLANGD_COMMANDS.extend(mod.perform_build(CLANGD_PATH, False))


''' Сборка ISO limine '''
def build_iso_limine():
    print("Сборка ISO limine")
    LIMINE_LIST = [
        "BOOTIA32.EFI", "BOOTX64.EFI", "limine-cd-efi.bin", 
        "limine.sys", "limine-cd.bin"
    ]

    if not os.path.exists("isodir/limine-cd.bin"):
        os.system(
                "git clone https://github.com/limine-bootloader/limine.git" \
                " --branch=v3.0-branch-binary" \
                " --depth=1")
        for i in LIMINE_LIST:
            shutil.copy(f"limine/{i}", "isodir/")
    
    exec_cmd("""xorriso -as mkisofs -b limine-cd.bin \
          -no-emul-boot -boot-load-size 4 -boot-info-table \
          --efi-boot limine-cd-efi.bin \
          -efi-boot-part --efi-boot-image --protective-msdos-label \
          isodir -o SynapseOS-limine.iso""")
    
    os.system(f"{LIMINE_DEPLOY} SynapseOS-limine.iso")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='SynapseOS build helper')
    parser.add_argument('-compiler', help='build with your compiler')
    parser.add_argument('-linker', help='build with your linker')
    parser.add_argument('-noqemu', help='Work without qemu')
    parser.add_argument('-limine', help='build with your limine-deploy')
    parser.add_argument('-docs',   help='build with generate docs(doxygen)')
    parser.add_argument('-clangd', help='generate clangd index database (compile_commands.json) to specified path')
    args = parser.parse_args()

    if args.compiler != None:
        CC = args.compiler

    if args.linker != None:
        LD = args.linker

    if args.limine != None:
        LIMINE_DEPLOY = args.limine

    CLANGD_PATH = args.clangd

    start_time = time.time()
    build_kernel()
    print(f"Сборка ядра заняла: {(time.time() - start_time):2f} секунд.")

    start_time = time.time()
    build_modules()
    print(f"Сборка модулей заняла: {(time.time() - start_time):2f} секунд.")

    if CLANGD_PATH:
        start_time = time.time()
        with open(os.path.join(CLANGD_PATH, 'compile_commands.json'), 'w') as f:
            f.write(json.dumps(CLANGD_COMMANDS))
        print(f'Генерация compile_commands.json заняла: {(time.time() - start_time):2f} секунд.')
    
    start_time = time.time()
    build_iso_limine()
    print(f"Сборка ISO//Limine образа заняла: {(time.time() - start_time):2f} секунд.")
    
    
    print(f"{ARCH}-elf-readelf -hls isodir/boot/kernel.elf>kernel.elf.txt")
    os.system(f"{ARCH}-elf-readelf -hls isodir/boot/kernel.elf>kernel.elf.txt")

    if args.docs != None:
        start_time = time.time()
        build_docs()
        print(f"Проверка и генерация документации заняла: {(time.time() - start_time):2f} секунд.")


    if args.noqemu == 0 or args.noqemu == None:
        if ARCH == "i686":
            QEMU_DEV = "-device rtl8139,id=nic0"
            QEMU = f"qemu-system-i386 -m 256 -d guest_errors -no-reboot {QEMU_DEV} " # -cpu pentium3
            
            exec_cmd(f"{QEMU} -monitor stdio -cdrom SynapseOS-limine.iso -serial file:serial.log")
