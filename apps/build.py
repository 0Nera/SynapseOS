import os, shutil, sys, tarfile, os.path, subprocess

LD = "ld.lld"
LDFLAGS = " -nostdlib -e main -o"
CC = "clang -target i386-pc-none-elf"
CFLAGS = " -Wno-unused-command-line-argument -mno-sse -mno-avx -O0 -ffreestanding -I include/ -c"

BUILD_BASIC = False # Change this to enable or disable BASIC build.

CC = f"{CC} {CFLAGS}"
LD = f"{LD} {LDFLAGS}"

print("[\x1b[32;1mCONFIGURATION\x1b[0m] Using C compiler:", CC)
print("[\x1b[32;1mCONFIGURATION\x1b[0m] Using linker:", LD)

AR = "llvm-ar"
_O_LIBC = ["./bin/libc/stdio.o", "./bin/libc/math.o", 
          "./bin/libc/ports.o","./bin/libc/stdlib.o","./bin/libc/string.o",
          "./bin/libc/learntask.o","./bin/libc/vesa.o","./bin/libc/scancodes.o"]
O_LIBC = ' ' + ' '.join(_O_LIBC)
data = []
files = []


# Сборка
def build(typ, infile, outfile):
    if typ=="compile":
        print(f"[\x1b[32;1mBUILD\x1b[0m]: Compiling {infile}")
        cmd = f"{CC} {infile} -o {outfile}"
        subprocess.call(cmd, shell=True)
    if typ=="link":
        # print(f"[\x1b[32mBUILD\x1b[0m]: Linking   {infile.split(" ")[0]}") # Error
        print(f"[\x1b[32;1mBUILD\x1b[0m]: Linking  ", infile.split(" ")[0])
        cmd = f"{LD} {outfile} {infile}"
        subprocess.call(cmd, shell=True)
    if typ=="copy":
        print(f"[\x1b[34;1mCOPY\x1b[0m]: Copying  ", infile+" -> "+outfile)
        shutil.copy(infile, outfile)


def build_all():
    try:
        shutil.rmtree("../bin/apps/", ignore_errors=True)
        shutil.rmtree("./bin", ignore_errors=True)
        os.mkdir("./bin")
        os.mkdir("./bin/libc")
        os.mkdir("../bin/apps/")
    except Exception as E:
        print(E)
    
    print("[\x1b[33;1mNOTICE\x1b[0m] Building apps")
    '''
    os.system(f"{CC} examples/C/HelloWorld.c -o ./bin/HelloWorld.o")
    os.system(f"{CC} examples/C/sound.c -o ./bin/beep.o")
    os.system(f"{CC} examples/C/popsort_int_test.c -o ./bin/popsort_int_test.o")
    os.system(f"{CC} examples/C/vesa_Russia.c -o ./bin/vesa_Russia.o")
    os.system(f"{CC} examples/C/char.c -o ./bin/char.o")
    os.system(f"{CC} apps/0Nera/NeraMath/main.c -o ./bin/NeraMath.o")
    os.system(f"{CC} apps/0Nera/VGGI/main.c -o ./bin/VGGI.o")
    os.system(f"{CC} games/kernelmode_power/snake/snake.c -o ./bin/snake.o")
    os.system(f"{CC} examples/C/test.c -o ./bin/test.o")
    '''

    build("compile", "examples/C/HelloWorld.c", "./bin/HelloWorld.o")
    build("compile", "examples/C/sound.c", "./bin/beep.o")
    build("compile", "examples/C/popsort_int_test.c", "./bin/popsort_int_test.o")
    build("compile", "examples/C/vesa_Russia.c", "./bin/vesa_Russia.o")
    build("compile", "examples/C/char.c", "./bin/char.o")
    build("compile", "apps/0Nera/NeraMath/main.c", "./bin/NeraMath.o")
    build("compile", "apps/0Nera/VGGI/main.c", "./bin/VGGI.o")
    build("compile", "games/kernelmode_power/snake/snake.c", "./bin/snake.o")
    build("compile", "examples/C/test.c", "./bin/test.o")
    build("compile", "apps/NDRAEY/ImageView/imageview.c", "./bin/imageview.o")
    build("compile", "apps/NDRAEY/Melody/melody.c", "./bin/melody.o")
    build("compile", "examples/gas/chronicles.asm", "./bin/chronicles.o")

    print("[\x1b[33;1mNOTICE\x1b[0m] Building libc")
    '''
    build("copy", "../kernel/src/libk/math.c", "libc/math.c")
    build("copy", "../kernel/include/libk/math.h", "include/math.h")
    '''
    build("compile", "libc/math.c", "./bin/libc/math.o")
    build("compile", "libc/stdio.c", "./bin/libc/stdio.o")
    build("compile", "libc/ports.c", "./bin/libc/ports.o")
    build("compile", "libc/stdlib.c", "./bin/libc/stdlib.o")
    build("compile", "libc/string.c", "./bin/libc/string.o")
    build("compile", "libc/learntask.c", "./bin/libc/learntask.o")
    build("compile", "libc/vesa.c", "./bin/libc/vesa.o")
    build("compile", "libc/scancodes.c", "./bin/libc/scancodes.o")

    print("[\x1b[33;1mNOTICE\x1b[0m] Linking apps")
    # There we should swap the second and third arguments (acctording to original code)
    build("link", "./bin/HelloWorld.o" + O_LIBC, "../bin/apps/hi")
    build("link", "./bin/popsort_int_test.o" + O_LIBC, "../bin/apps/sort")
    build("link", "./bin/vesa_Russia.o" + O_LIBC, "../bin/apps/Russia", )
    build("link", "./bin/beep.o" + O_LIBC, "../bin/apps/beep")
    build("link", "./bin/char.o" + O_LIBC, "../bin/apps/char")
    build("link", "./bin/NeraMath.o" + O_LIBC, "../bin/apps/NeraMath")
    build("link", "./bin/VGGI.o" + O_LIBC, "../bin/apps/vggi")
    build("link", "./bin/snake.o" + O_LIBC, "../bin/apps/snake")
    build("link", "./bin/test.o", "../bin/apps/test")
    build("link", "./bin/imageview.o" + O_LIBC, "../bin/apps/imageview")
    build("link", "./bin/melody.o" + O_LIBC, "../bin/apps/melody")
    build("link", "./bin/chronicles.o" + O_LIBC, "../bin/apps/chronicles")

    try:
        if shutil.which("fasm"):
            os.system("fasm examples/fasm/hello.asm ./bin/asm.o")
            build("link", "./bin/asm.o", "../bin/apps/asm")
        else:
            print("[\x1b[31;1mERROR\x1b[0m] Binary 'fasm' not found, skipping...")
    except Exception:
        pass

    if BUILD_BASIC:
        print(("="*20)+"[Building BASIC]"+("="*20))
        os.chdir("basic-prog-lang");
        subprocess.call("make", shell = True)
        files = subprocess.Popen("make synchronize", shell = True, stdout = subprocess.PIPE).stdout.read()[:-1]
        files = files.decode("utf-8").split(" ")
        os.chdir("..")
        for n, i in enumerate(files):
            files[n] = "basic-prog-lang/"+i
        files = ' '.join(files)
        
        cmd = f"{LD} basic {files} {O_LIBC}"
        print(f"[\x1b[33;1mNOTICE\x1b[0m] Running command: {cmd}")
        subprocess.call(cmd, shell=True)

    
if __name__ == "__main__":
    try:
        build_all()
    except Exception as E:
        print(E)
        input()
