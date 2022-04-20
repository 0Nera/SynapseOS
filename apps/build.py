import os, shutil, sys, tarfile, os.path

CCFLAGS = "-g -I include/ -ffreestanding -Wall -Wextra -w -O0"
LD = "-nostdlib -lgcc -T link.ld -o"
CC = "i686-elf-gcc " + CCFLAGS
data = []
files = []


def build_all():
    try:
        shutil.rmtree("./bin", ignore_errors=True)
        os.mkdir("./bin")
    except Exception as E:
        print(E)
    
    print("Building apps")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/HelloWorld.c -o ./bin/HelloWorld.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/sound.c -o ./bin/beep.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/popsort_int_test.c -o ./bin/popsort_int_test.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/vesa_Russia.c -o ./bin/vesa_Russia.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/char.c -o ./bin/char.o")
    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/C/scancodes.c -o ./bin/scancodes.o")

    os.system("i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c examples/drivers/floppy.c -o ./bin/floppy.o")
    
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/hi.elf ./bin/HelloWorld.o")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/sort.elf ./bin/popsort_int_test.o")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/Russia.elf ./bin/vesa_Russia.o")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/beep.elf ./bin/beep.o")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/char.elf ./bin/char.o")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/scancodes.elf ./bin/scancodes.o")
    os.system("i686-elf-gcc -nostdlib -lgcc -T link.ld -o ../bin/apps/floppy.elf ./bin/floppy.o")


if __name__ == "__main__":
    try:
        build_all()
    except Exception as E:
        print(E)
        input()