import os, shutil, sys, tarfile, os.path

LD = "ld.lld -T link.ld -nostdlib -o"
CC = "clang -target i386-pc-none-elf -w -mno-sse -mno-avx -O0 -ffreestanding -I include/ -c"
AR = "llvm-ar"
O_LIBC = " ./bin/libc/stdio.o ./bin/libc/ports.o ./bin/libc/stdlib.o ./bin/libc/string.o ./bin/libc/learntask.o ./bin/libc/vesa.o ./bin/libc/scancodes.o"
data = []
files = []


def build_all():
    try:
        shutil.rmtree("../bin/apps/", ignore_errors=True)
        shutil.rmtree("./bin", ignore_errors=True)
        os.mkdir("./bin")
        os.mkdir("./bin/libc")
        os.mkdir("../bin/apps/")
    except Exception as E:
        print(E)
    
    print("Building apps")
    os.system(f"{CC} examples/C/HelloWorld.c -o ./bin/HelloWorld.o")
    os.system(f"{CC} examples/C/sound.c -o ./bin/beep.o")
    os.system(f"{CC} examples/C/popsort_int_test.c -o ./bin/popsort_int_test.o")
    os.system(f"{CC} examples/C/vesa_Russia.c -o ./bin/vesa_Russia.o")
    os.system(f"{CC} examples/C/char.c -o ./bin/char.o")
    os.system(f"{CC} apps/0Nera/NeraMath/main.c -o ./bin/NeraMath.o")
    os.system(f"{CC} apps/0Nera/VGGI/main.c -o ./bin/VGGI.o")
    os.system(f"{CC} games/kernelmode_power/snake/snake.c -o ./bin/snake.o")
    os.system(f"{CC} examples/C/test.c -o ./bin/test.o")

    print("Building libc")
    os.system(f"{CC} libc/stdio.c -o ./bin/libc/stdio.o")
    os.system(f"{CC} libc/ports.c -o ./bin/libc/ports.o")
    os.system(f"{CC} libc/stdlib.c -o ./bin/libc/stdlib.o")
    os.system(f"{CC} libc/string.c -o ./bin/libc/string.o")
    os.system(f"{CC} libc/learntask.c -o ./bin/libc/learntask.o")
    os.system(f"{CC} libc/vesa.c -o ./bin/libc/vesa.o")
    os.system(f"{CC} libc/scancodes.c -o ./bin/libc/scancodes.o")
    
    print("Linking apps")
    os.system(f"{LD} ../bin/apps/hi ./bin/HelloWorld.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/sort ./bin/popsort_int_test.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/Russia ./bin/vesa_Russia.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/beep ./bin/beep.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/char ./bin/char.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/NeraMath ./bin/NeraMath.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/vggi ./bin/VGGI.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/snake ./bin/snake.o" + O_LIBC)
    os.system(f"{LD} ../bin/apps/test ./bin/test.o")

if __name__ == "__main__":
    try:
        build_all()
    except Exception as E:
        print(E)
        input()