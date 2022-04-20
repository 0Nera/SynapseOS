#include <stdio.h>
#include <ports.h>


int main(){
    printf("\nEnter ` to break.\n");

    int c;

    while(1){
        c = getscancode();

        if (c == 27){
            break;
        } else {
            printf("%d\n", c);
        }
    }

    return 0;
}