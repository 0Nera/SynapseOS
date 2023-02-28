#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Сборщик модулей SynapseOS

(c) Арен Елчинян
"""

import os
import json




ARCH = "i686" # "x86_64", "arm", "e2k"
ARCH_DIR = "x86" # "x86_64", "arm", "e2k"

DEBUG_FLAGS = "-D DEBUG=1"

CC = f"{ARCH}-elf-gcc"

MOD_PROTECT = "-fno-builtin -fno-stack-protector"
MOD_FLAGS = f"-I mod/include/ -ffreestanding -m32 -nostdlib -nostartfiles {MOD_PROTECT} -e main"


def scan_all():
    LIST_DIRS = []

    for f in os.scandir("mod/"):
        if f.is_dir():
            if f.path == "mod/include":
                continue
            LIST_DIRS.append(f.path)

    return LIST_DIRS


def scan_target_dir(dir):
    TARGET = []
    for file in os.scandir(dir):
        if file.is_file():
            TARGET.append(file.path)
        else:
            for T in scan_target_dir(file.path):
                TARGET.append(T)
    return TARGET


def parse_config(mod_dir):
    try:
        with open(f"{mod_dir}/build.json", "r") as f:
            conf = json.loads(f.read())
            name = conf['name']
            version = conf['version']
            author = conf['author']
            permissions = conf['permissions']
            linker = conf['linker']
            header = conf['header']
            assets = conf['assets']
            source = f"{mod_dir}/{conf['source']}"
            output = conf['output']
            TARGETS = scan_target_dir(source)
            for T in TARGETS:
                t = T.replace('/', '_').replace('\\', '_')
                print(f"{CC} -D__NAME__=\"{name}\" {MOD_FLAGS} -c {T} -o bin/{t}.o")
                os.system(f"{CC} -D__NAME__=\"{name}\" {MOD_FLAGS} -c {T} -o bin/{t}.o")
            os.system(f"{CC} {MOD_FLAGS} -o isodir/modules/{output} bin/simple.o")
            
            print(f"{ARCH}-elf-readelf -hls isodir/modules/{output}>debug/{output}.elf.txt")
            os.system(f"{ARCH}-elf-readelf -hls isodir/modules/{output}>debug/{output}.elf.txt")
            
    except Exception as E:
        print(E)


if __name__ == '__main__':
    LIST_DIRS = scan_all()

    for dir in LIST_DIRS:
        parse_config(f"{dir}")