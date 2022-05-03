#include <stdio.h>


int main(){
    printf("\nEnter 10 characters to break.\n");

    int i = 0;
    while(1){
        int c = getscancode();

        if (++i == 10){
            printf("%d.", c);
            break;
        } else {
            printf("%d, ", c);
        }
    }

    return 0;
}