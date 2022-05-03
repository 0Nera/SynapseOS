#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_version         40


int print_str(char *str) {
    void* res = 8;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_puts), "b"(&str));
    asm volatile("int $0x80");
    return res;
}


int getversion(){
    void* res = 8;
    int result = -1;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_version));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));
    return result;
}


int getscancode(){
    void* res = 8;

    asm volatile("mov %%eax, %0" : "=a"(res) : "a"(SC_CODE_getscancode));
    asm volatile("int $0x80");
    return res;
}


int main() {
    print_str("*****TEST ALL SYSCALLS*****\n");
    print_str("Try get version\n");

    if (getversion() == 26){
        print_str("RIGHT!\n");
        return 1;
    }

    print_str("FAIL!\n");
    
    return -3;
}