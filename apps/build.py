import os, shutil, sys, tarfile, os.path, subprocess

LD = "ld.lld"
LDFLAGS = "-T link.ld -nostdlib -o"
CC = "clang -target i386-pc-none-elf"
CFLAGS = "-w -mno-sse -mno-avx -O0 -ffreestanding -I include/ -c"

BUILD_LUA = True  # Change this to enable or disable Lua build.

CC = f"{CC} {CFLAGS}"
LD = f"{LD} {LDFLAGS}"

print("[\x1b[32mCONFIGURATION\x1b[0m] Using C compiler:", CC)
print("[\x1b[32mCONFIGURATION\x1b[0m] Using linker:", LD)

AR = "llvm-ar"
O_LIBC = [
    "./bin/libc/stdio.o",
    "./bin/libc/stdfile.o",
    "./bin/libc/ports.o",
    "./bin/libc/stdlib.o",
    "./bin/libc/string.o",
    "./bin/libc/learntask.o",
    "./bin/libc/vesa.o",
    "./bin/libc/scancodes.o",
]
O_LIBC = " " + " ".join(O_LIBC)
data = []
files = []


def build(typ, infile, outfile):
    if typ == "compile":
        print(f"[\x1b[32mBUILD\x1b[0m]: Compiling {infile}")
        cmd = f"{CC} {infile} -o {outfile}"
        subprocess.call(cmd, shell=True)
    if typ == "link":
        # print(f"[\x1b[32mBUILD\x1b[0m]: Linking   {infile.split(" ")[0]}") # Error
        print(f"[\x1b[32mBUILD\x1b[0m]: Linking  ", infile.split(" ")[0])
        cmd = f"{LD} {outfile} {infile}"
        subprocess.call(cmd, shell=True)


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
    """
    os.system(f"{CC} examples/C/HelloWorld.c -o ./bin/HelloWorld.o")
    os.system(f"{CC} examples/C/sound.c -o ./bin/beep.o")
    os.system(f"{CC} examples/C/popsort_int_test.c -o ./bin/popsort_int_test.o")
    os.system(f"{CC} examples/C/vesa_Russia.c -o ./bin/vesa_Russia.o")
    os.system(f"{CC} examples/C/char.c -o ./bin/char.o")
    os.system(f"{CC} apps/0Nera/NeraMath/main.c -o ./bin/NeraMath.o")
    os.system(f"{CC} apps/0Nera/VGGI/main.c -o ./bin/VGGI.o")
    os.system(f"{CC} games/kernelmode_power/snake/snake.c -o ./bin/snake.o")
    os.system(f"{CC} examples/C/test.c -o ./bin/test.o")
    """

    build("compile", "examples/C/HelloWorld.c", "./bin/HelloWorld.o")
    build("compile", "examples/C/sound.c", "./bin/beep.o")
    build("compile", "examples/C/popsort_int_test.c", "./bin/popsort_int_test.o")
    build("compile", "examples/C/vesa_Russia.c", "./bin/vesa_Russia.o")
    build("compile", "examples/C/char.c", "./bin/char.o")
    build("compile", "apps/0Nera/NeraMath/main.c", "./bin/NeraMath.o")
    # build("compile", "apps/0Nera/VGGI/main.c", "./bin/VGGI.o")
    build("compile", "games/kernelmode_power/snake/snake.c", "./bin/snake.o")
    build("compile", "examples/C/test.c", "./bin/test.o")
    build("compile", "apps/NDRAEY/ImageView/imageview.c", "./bin/imageview.o")

    print("Building libc")
    build("compile", "libc/stdio.c", "./bin/libc/stdio.o")
    build("compile", "libc/ports.c", "./bin/libc/ports.o")
    build("compile", "libc/stdlib.c", "./bin/libc/stdlib.o")
    build("compile", "libc/stdfile.c", "./bin/libc/stdfile.o")
    build("compile", "libc/string.c", "./bin/libc/string.o")
    build("compile", "libc/learntask.c", "./bin/libc/learntask.o")
    build("compile", "libc/vesa.c", "./bin/libc/vesa.o")
    build("compile", "libc/scancodes.c", "./bin/libc/scancodes.o")

    print("Linking apps")
    # There we should swap the second and third arguments (acctording to original code)
    build("link", "./bin/HelloWorld.o" + O_LIBC, "../bin/apps/hi")
    build("link", "./bin/popsort_int_test.o" + O_LIBC, "../bin/apps/sort")
    build(
        "link",
        "./bin/vesa_Russia.o" + O_LIBC,
        "../bin/apps/Russia",
    )
    build("link", "./bin/beep.o" + O_LIBC, "../bin/apps/beep")
    build("link", "./bin/char.o" + O_LIBC, "../bin/apps/char")
    build("link", "./bin/NeraMath.o" + O_LIBC, "../bin/apps/NeraMath")
    # build("link", "./bin/VGGI.o" + O_LIBC, "../bin/apps/vggi")
    build("link", "./bin/snake.o" + O_LIBC, "../bin/apps/snake")
    build("link", "./bin/test.o", "../bin/apps/test")
    build("link", "./bin/imageview.o" + O_LIBC, "../bin/apps/imageview")

    """
    if BUILD_LUA:
        print(("="*20)+"[Building Lua]"+("="*20))
        os.chdir("lua");
        # subprocess.call("make -j2", shell = True)
    """


if __name__ == "__main__":
    try:
        build_all()
    except Exception as E:
        print(E)
        input()
