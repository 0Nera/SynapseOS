#include <stdio.h>


int main(){

    int i = 10;

    while(i){
        char c = getchar();
        printf("[%c %d] ", c, c);
        i--;
    }

    return 0;
}