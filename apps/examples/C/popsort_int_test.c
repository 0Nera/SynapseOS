#include <stdio.h>


int main(){
    printf("\nPopsort test:");
    
    int test[] = {32, 4, 8, 0};
    int n = sizeof(test) / sizeof(int);
    
    for(int i = 0 ; i < n - 1; i++) { 
       // сравниваем два соседних элемента.
       for(int j = 0 ; j < n - i - 1 ; j++) {  
           if(test[j] > test[j+1]) {           
              // если они идут в неправильном порядке, то  
              //  меняем их местами. 
              int tmp = test[j];
              test[j] = test[j+1] ;
              test[j+1] = tmp; 
           }
        }
    }

    for (int i = 0; i != sizeof(test) / sizeof(int); i++){
        printf("\n%d: %d", i, test[i]);
    }
    return 0;
}