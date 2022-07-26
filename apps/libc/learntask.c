#include <learntask.h>


int fibonacci(int N) {
    if (N == 1 || N == 2)
        return 1; // первые 2 числа равны 1
    return fibonacci(N - 1) + fibonacci(N - 2); // складываем предыдущие 2 числа
}
