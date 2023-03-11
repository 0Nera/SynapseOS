#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Сборщик модулей SynapseOS

(c) Арен Елчинян
"""

import os
import json
import re
import sys
import argparse
import time




ARCH = "i686" # "x86_64", "arm", "e2k"
ARCH_DIR = "x86" # "x86_64", "arm", "e2k"

DEBUG_FLAGS = "-D DEBUG=1"

CC = f"{ARCH}-elf-gcc"

MOD_PROTECT = "-fno-builtin -fno-stack-protector"
MOD_FLAGS = f"-I mod/include/ -ffreestanding -m32 -nostdlib -nostartfiles {MOD_PROTECT} -e main"

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


def scan_all():
    LIST_DIRS = []

    for f in os.scandir("mod/"):
        if f.is_dir():
            if f.path == "mod/include":
                continue
            LIST_DIRS.append(f.path)

    return LIST_DIRS


def scan_target_dir(dir):
    regexp = re.compile(r'\.(c|cpp)$')
    print(dir)
    for file in os.scandir(dir):
        if file.is_file():
            if regexp.search(file.name):
                yield file.path
        else:
            yield from scan_target_dir(file.path)


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
            sources = []
            for T in TARGETS:
                t = re.sub(r'/|\\', '_', T)
                sources.append(f'bin/{t}.o')
                exec_cmd(f"{CC} -D __NAME__='\"{name}\"' {MOD_FLAGS} -c {T} -o bin/{t}.o")
            exec_cmd(f"{CC} {MOD_FLAGS} -o isodir/modules/{output} {' '.join(sources)}")
            os.makedirs('debug', exist_ok=True)
            exec_cmd(f"{ARCH}-elf-readelf -hls isodir/modules/{output} > debug/{output}.elf.txt")
            
    except Exception as E:
        print(E.with_traceback())


def perform_build(clangd=None, write_clangd=True):
    global CLANGD_PATH
    CLANGD_PATH = clangd
    for d in scan_all():
        if d == 'mod/__pycache__':
            continue
        parse_config(str(d))

    if CLANGD_PATH:
        if write_clangd:
            start_time = time.time()
            with open(os.path.join(CLANGD_PATH, 'compile_commands.json'), 'w') as f:
                f.write(json.dumps(CLANGD_COMMANDS))
            print(f'Генерация compile_commands.json заняла: {(time.time() - start_time):2f} секунд.')
        else:
            return CLANGD_COMMANDS


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='SynapseOS build helper for modules')
    parser.add_argument('-clangd', help='generate clangd index database (compile_commands.json) to specified path')
    args = parser.parse_args()
    perform_build(args.clangd)
