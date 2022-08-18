#include "../include/stdio.h"
#include "../include/string.h"

int main() {
    printf("Memtest by NDRAEY\n\nChecking memory...\n");

    printf("Step 1: Allocating 10 bytes of memory...\n");
    char* mem = malloc(10);
    if(!mem) { printf("malloc() failed, because mem==0\n"); return 1; }

    printf("Writing pattern 11 22 33 44 55 66 77 88 99 0...\n");
    mem[0] = 11;
    mem[1] = 22;
    mem[2] = 33;
    mem[3] = 44;
    mem[4] = 55;
    mem[5] = 66;
    mem[6] = 77;
    mem[7] = 88;
    mem[8] = 99;
    mem[9] = 0;

    printf("Written pattern is: ");
    
    for(int i=0; i<10; i++) {
        printf("%d ", mem[i]);
    }
    printf("\nFreeing...\n");
    free(mem);

    printf("NOTE: Patterns should be same\n");
    printf("================\n");
    char* memory = 0;

    printf("Step 2: Allocating and freeing memory 1000 times... (128 bytes)\n");
    memory = malloc(128);
    if(!memory) { printf("malloc() failed, because memory==0\n"); return 1; }
    printf("Memory start is: %d\n", memory);

    for(int z=0; z<1000; z++) {
        free(memory);
        memory = malloc(128);
    }

    printf("Memory end is: %d\n", memory);
    printf("NOTE: Start and End of memory should be same\n");
    free(memory);

    printf("================\n");
    memory = 0;

    printf("Step 3: Allocating and freeing memory 1M times... (1024 bytes)\n");
    memory = malloc(1024);
    if(!memory) { printf("malloc() failed, because memory==0\n"); return 1; }
    printf("Memory start is: %d\n", memory);

    for(int z=0; z<1000000; z++) {
        free(memory);
        memory = malloc(1024);
    }

    printf("Memory end is: %d\n", memory);
    printf("NOTE: Start and End of memory should be same\n");
    free(memory);
    printf("================\n");

    printf("Step 4: memset() test... Pattern 49 for 1024 bytes => ");
    memory = 0;
    memory = malloc(1024);
    if(!memory) { printf("malloc() failed, because memory==0\n"); return 1; }

    memset(memory, 49, 1024);

    for(int i=0; i<1024; i++) {
        if(memory[i]!=49) {
            printf("FAIL!\n\nERROR: memset may be buggy or memory error.\n");
            printf("Bug encountered at position %d\n", i);
            return 1;
        }
    }

    printf("PASS!\n");

    printf("================\n");

    return 0;
}