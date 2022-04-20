#include <stdio.h>
#include <ports.h>


int main(){
    printf("\nEnter 'e' to break.\n");

    char c;

    while(1){
        c = getchar();

        if (c == 'e'){
            break;
        } else {
            printf("%c\n", c);
        }
    }

    return 0;
}