import os, shutil, sys, tarfile, os.path

CCFLAGS = "-g -I include/ -ffreestanding -Wall -Wextra -w -O0"
LD = "-nostdlib -lgcc -T link.ld -o"
CC = "i686-elf-gcc " + CCFLAGS
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
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/HelloWorld.c -o ./bin/HelloWorld.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/sound.c -o ./bin/beep.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/popsort_int_test.c -o ./bin/popsort_int_test.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/vesa_Russia.c -o ./bin/vesa_Russia.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/char.c -o ./bin/char.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c apps/0Nera/NeraMath/main.c -o ./bin/NeraMath.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/test.c -o ./bin/test.o")

    print("Building libc")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/stdio.c -o ./bin/libc/stdio.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/ports.c -o ./bin/libc/ports.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/stdlib.c -o ./bin/libc/stdlib.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/string.c -o ./bin/libc/string.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/learntask.c -o ./bin/libc/learntask.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/vesa.c -o ./bin/libc/vesa.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/scancodes.c -o ./bin/libc/scancodes.o")

    
    print("Linking apps")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/hi.elf ./bin/HelloWorld.o" + O_LIBC)
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/sort.elf ./bin/popsort_int_test.o" + O_LIBC)
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/Russia.elf ./bin/vesa_Russia.o" + O_LIBC)
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/beep.elf ./bin/beep.o" + O_LIBC)
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/char.elf ./bin/char.o" + O_LIBC)
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/NeraMath.elf ./bin/NeraMath.o" + O_LIBC)
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/test.elf ./bin/test.o")

if __name__ == "__main__":
    try:
        build_all()
    except Exception as E:
        print(E)
        input()