#include <learntask.h>


int fibonacci(int N) {
    if (N == 1 || N == 2)
        return 1; // первые 2 числа равны 1
    return fibonacci(N - 1) + fibonacci(N - 2); // складываем предыдущие 2 числа
}


void popsort_int(int a[]) {
    // считываем количество чисел n
    int n = sizeof(a) / sizeof(int);
    int temp[n];

    for(int i = 0; i != n; i++){
        temp[i] = a[i];
        printf("\n%d: %d", i, temp[i]);
    }

    for(int i = 0 ; i < n - 1; i++) { 
       // сравниваем два соседних элемента.
       for(int j = 0 ; j < n - i - 1 ; j++) {  
           if(a[j] > a[j+1]) {           
              // если они идут в неправильном порядке, то  
              //  меняем их местами. 
              int tmp = temp[j];
              temp[j] = temp[j+1] ;
              temp[j+1] = tmp; 
           }
        }
    }

    for (int i = 0; i != sizeof(temp) / sizeof(int); i++){
        printf("\n%d: %d", i, temp[i]);
    }
 }