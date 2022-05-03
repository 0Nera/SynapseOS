#include <stdio.h>


void setcur(int x, int y) {
}


int main() {
    // создаем все нужные переменные
    char mas[10][21];
    int i;
    int x = 10,y = 5;
    char key;
    int ox,oy;
    int ax = 5,ay = 3;
    int apple = 0;

    // переменные для хвоста

    // создаем генерацию рандомного числа
    srand(time(NULL));

    do
    {
        // визуализация поля, собаки и яблок
    printf(mas[0], "####################");
    for (i = 1; i < 9; i++)
        printf(mas[i], "#                  #");
    printf(mas[9], "####################");

    mas[y][x] = '@';
    mas[ay][ax] = '*';

    setcur(0,0); // очистка экрана
    for (i = 0; i < 10; i++)
        printf("%s\n", mas[i]);

    printf("\n apples: %d", apple);

    // движение собаки
    key = getch();
    ox = x;
    oy = y;

    if (GetKeyState('W') < 0) y--;
    if (GetKeyState('S') < 0) y++;
    if (GetKeyState('A') < 0) x--;
    if (GetKeyState('D') < 0) x++;
    if (mas[y][x] == '#')
    {
        x = ox;
        y = oy;
    }
    if ((x == ax) && (y == ay))
    {
        ax = rand() * 1.0 / RAND_MAX * 18 + 1;
        ay = rand() * 1.0 / RAND_MAX * 8 + 1;
        apple++;
    }

    }
    while (key != 'e');

    return 0;
}
