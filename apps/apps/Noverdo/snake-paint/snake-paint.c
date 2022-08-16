/*
    Snake Paint
    Автор: Noverdo
    Спасибо за код оригинальной snakegame kenrelpower(GPLv3)
    GPL v3
*/

#include <stdio.h>
#include <vesa.h> 
#include <ports.h>

#define CELL_SIZE 20
#define MAP_W 50
#define MAP_H 50
#define MAP_POS_X 50
#define MAP_POS_Y 50
#define MAX_SNAKE_LEN 100
#define COLOR_BACK VESA_BLUE
#define COLOR_BLACK VESA_BLACK
#define COLOR_SNAKE VESA_LIGHT_GREEN
#define COLOR_FOOD VESA_BROWN

typedef struct {
    int x, y;
} pair_t;
typedef pair_t point_t;
typedef pair_t vector_t;

typedef struct {
    vector_t direction;
    point_t body[MAX_SNAKE_LEN]; // Нулевой эллемент - голова змейки
    int length;
} snake_t;

/*
static unsigned long int next_rnd = 1;

int rand(void) {
    next_rnd = next_rnd * 1103515245 + 12345;
    return (unsigned int)(next_rnd/65536) % 32768;
}

void srand(unsigned int seed) {
    next_rnd = seed;
}*/

void draw_square(int x, int y, int w, int h, int color) {
    for (int j = x; j < x + w; j++) {
        for (int i = y; i < y + h; i++){
            draw_pixel(j, i, color);
        }
    }
}


int main(void) {

    srand(1337);
    printf("For control using wasd keys\nFor exit using q key");
    sleep_ms(3000);

    // инициализация игровой карты
    draw_square(MAP_POS_X, MAP_POS_Y, MAP_W*CELL_SIZE, MAP_H*CELL_SIZE, COLOR_BLACK);

    snake_t snake;
    snake.direction = (vector_t){1, 0};
    snake.length = 1;
    snake.body[0] = (point_t){15, 25};
    snake.body[1] = (point_t){14, 25};
    snake.body[2] = (point_t){14, 26};
    snake.body[3] = (point_t){14, 27};
    snake.body[4] = (point_t){14, 28};
    int score = 0;
    // Инициализация змеи
    for (int i = 0; i < snake.length; i++) {
        draw_square(MAP_POS_X + snake.body[i].x*CELL_SIZE,
            MAP_POS_Y + snake.body[i].y*CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            COLOR_SNAKE
        );
    }
 
    bool counti = true; 
    while (1) {
    	if (counti) {
        // Прорисовка за змейкой
        draw_square(MAP_POS_X + snake.body[snake.length - 1].x*CELL_SIZE,
            MAP_POS_Y + snake.body[snake.length - 1].y*CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            COLOR_BACK
        );
        int new_head_x = snake.body[0].x + snake.direction.x;
        int new_head_y = snake.body[0].y + snake.direction.y;
        // Отросивка движа змейки
        for (int i = snake.length - 1; i > 0; i--) {
            snake.body[i] = snake.body[i - 1];
            }
        snake.body[0] = (point_t){new_head_x, new_head_y};



        // нарисвать голову змеи
        draw_square(MAP_POS_X + snake.body[0].x*CELL_SIZE,
            MAP_POS_Y + snake.body[0].y*CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            COLOR_SNAKE
        );
        counti=false;
	}
	else {
		counti=true;
	}
        int key = getscancode();
        if (key == 0x11) { // w
            if (!(snake.direction.x == 0 && snake.direction.y == 1))
            snake.direction = (vector_t){0, -1};
        }
        if (key == 0x1E) { // a
        if (!(snake.direction.x == 1 && snake.direction.y == 0))
            snake.direction = (vector_t){-1, 0};
        }
        if (key == 0x1F) { // s
        if (!(snake.direction.x == 0 && snake.direction.y == -1))
            snake.direction = (vector_t){0, 1};
        }
        if (key == 0x20) { // d
        if (!(snake.direction.x == -1 && snake.direction.y == 0))
            snake.direction = (vector_t){1, 0};
        }
        if (key == 0x10) { // q для выхода
        	return 0;
        }

        sleep_ms(200);
    }

    return 0;
}

