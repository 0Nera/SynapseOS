/*
    Snake game for SynapseOS
    Цырен Цыбикжапов aka kernelmode_power (c)
    GPL v3
*/

#include <stdio.h>
#include <vesa.h> 
#include <ports.h>

#define CELL_SIZE 10
#define MAP_W 50
#define MAP_H 50
#define MAP_POS_X 380
#define MAP_POS_Y 50
#define MAX_SNAKE_LEN 100
#define COLOR_BACK VESA_BLUE
#define COLOR_SNAKE VESA_LIGHT_GREEN
#define COLOR_FOOD VESA_BROWN

typedef struct {
    int x, y;
} pair_t;
typedef pair_t point_t;
typedef pair_t vector_t;

typedef struct {
    vector_t direction;
    point_t body[MAX_SNAKE_LEN]; // 0th element - head
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

point_t gen_food_position(snake_t *snake) {
    for (;;) {
        point_t p = (point_t){rand() % MAP_W, rand() % MAP_H};
        int ok = 1;
        for (int i = 0; i < snake->length; i++) {
            if (p.x == snake->body[i].x && p.y == snake->body[i].y
                || p.x == MAP_W - 1 || p.y == MAP_H - 1)
            {
                ok = 0;
                break;
            }
        }
        if (ok) {
            return p;
        }
    }
    return (point_t){0, 0};
}

int main(void) {

    srand(1337);

    // draw game map
    draw_square(MAP_POS_X, MAP_POS_Y, MAP_W*CELL_SIZE, MAP_H*CELL_SIZE, COLOR_BACK);

    snake_t snake;
    snake.direction = (vector_t){1, 0};
    snake.length = 5;
    snake.body[0] = (point_t){15, 25};
    snake.body[1] = (point_t){14, 25};
    snake.body[2] = (point_t){14, 26};
    snake.body[3] = (point_t){14, 27};
    snake.body[4] = (point_t){14, 28};
    point_t food = gen_food_position(&snake);
    int score = 0;
    // initially draw whole snake
    for (int i = 0; i < snake.length; i++) {
        draw_square(MAP_POS_X + snake.body[i].x*CELL_SIZE,
            MAP_POS_Y + snake.body[i].y*CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            COLOR_SNAKE
        );
    }
    // and food
    draw_square(MAP_POS_X + food.x*CELL_SIZE,
        MAP_POS_Y + food.y*CELL_SIZE,
        CELL_SIZE,
        CELL_SIZE,
        COLOR_FOOD
    );

    while (1) {
        // erase tail
        draw_square(MAP_POS_X + snake.body[snake.length - 1].x*CELL_SIZE,
            MAP_POS_Y + snake.body[snake.length - 1].y*CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            COLOR_BACK
        );
        int new_head_x = snake.body[0].x + snake.direction.x;
        int new_head_y = snake.body[0].y + snake.direction.y;
        if (new_head_x >= 0 && new_head_x < MAP_W &&
            new_head_y >= 0 && new_head_y < MAP_H)
        {
            for (int i = snake.length - 1; i > 0; i--) {
                snake.body[i] = snake.body[i - 1];
            }
            snake.body[0] = (point_t){new_head_x, new_head_y};
        } else {
            printf("Game over! Your score is %d\n", score);
            return 0;
        }

        // check collision with own body
        for (int i = 1; i < snake.length; i++) {
            if (snake.body[0].x == snake.body[i].x
                && snake.body[0].y == snake.body[i].y)
            {
                printf("Game over! Your score is %d\n", score);
                return 0;
            }
        }

        // if we got to food
        if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
            // growth
            score++;
            snake.length++;
            snake.body[snake.length - 1] = snake.body[snake.length - 2];
            // generate new food and draw it
            food = gen_food_position(&snake);
            draw_square(MAP_POS_X + food.x*CELL_SIZE,
                MAP_POS_Y + food.y*CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                COLOR_FOOD
            );
        }

        // draw head on new position
        draw_square(MAP_POS_X + snake.body[0].x*CELL_SIZE,
            MAP_POS_Y + snake.body[0].y*CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            COLOR_SNAKE
        );

        int key = getscancode();
        // printf("key = %d %d\n", -1, key);
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

        sleep_ms(500);
    }

    return 0;
}


