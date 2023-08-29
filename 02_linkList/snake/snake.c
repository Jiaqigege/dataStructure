#include <curses.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#include "display.h"
#include "doublyLinkedList.h"

typedef struct SnakeBody {
    uint32_t x;
    uint32_t y;
} SnakeBody;

typedef struct Food {
    uint32_t x;
    uint32_t y;
} Food;

#define HEIGHT   (30)
#define WIDTH    (30)
#define CENTER_X (WIDTH / 2)
#define CENTER_Y (HEIGHT / 2)

enum Direction
{
    DIRECTION_UP = KEY_UP,
    DIRECTION_DOWN = KEY_DOWN,
    DIRECTION_LEFT = KEY_LEFT,
    DIRECTION_RIGHT = KEY_RIGHT,
};

DoublyLinkedList *snakeInit(int x, int y)
{
    int ret;

    DoublyLinkedList *snakeList = createDoublyLinkedList(sizeof(SnakeBody));
    if (!snakeList)
        return NULL;

    SnakeBody head = {.x = x, .y = y};
    ret = appendDoublyLinkedList(snakeList, &head);
    if (ret)
        return NULL;

    return snakeList;
}

void snakeDeinit(DoublyLinkedList *snake)
{
    destoryDoublyLinkedList(snake);
}

int match_food(const void *listValue, const void *compareValue)
{
    SnakeBody *snake = (SnakeBody *)listValue;
    Food *food = (Food *)compareValue;
    if (snake->x == food->x && snake->y == food->y) {
        return 0;
    }
    return -1;
}

int generate_food(DoublyLinkedList *snakeList, Food *food)
{
    if (food->x == -1 && food->y == -1) {
        while (1) {
            food->x = rand() % WIDTH;
            food->y = rand() % HEIGHT;
            if (food->x == 0 || food->y == 0)
                continue;

            if (matchElementDoublyLinkedList(snakeList, match_food, &food, NULL, NULL) != 0) {
                return 0;
            }
        }
    }
    return 0;
}

int calDirection(int x, int y)
{
    int direction = DIRECTION_LEFT;
    if (abs(x - CENTER_X) >= abs(y - CENTER_Y)) {
        // x方向更远或相等
        direction = x >= CENTER_X ? DIRECTION_LEFT : DIRECTION_RIGHT;
    } else {
        // y方向更远
        direction = y >= CENTER_Y ? DIRECTION_UP : DIRECTION_DOWN;
    }
    return direction;
}

int moveSnake(DoublyLinkedList *snakeList, Food *food, int dir)
{
    SnakeBody *snakeHead = snakeList->head->element;

    SnakeBody newLocation;
    switch (dir) {
    case DIRECTION_LEFT:
        newLocation.x = snakeHead->x - 1;
        if (newLocation.x == 0)
            newLocation.x = WIDTH - 1;
        newLocation.y = snakeHead->y;
        break;
    case DIRECTION_RIGHT:
        newLocation.x = snakeHead->x + 1;
        if (newLocation.x == WIDTH)
            newLocation.x = 1;
        newLocation.y = snakeHead->y;
        break;
    case DIRECTION_DOWN:
        newLocation.x = snakeHead->x;
        newLocation.y = snakeHead->y + 1;
        if (newLocation.y == HEIGHT)
            newLocation.y = 1;
        break;
    case DIRECTION_UP:
        newLocation.x = snakeHead->x;
        newLocation.y = snakeHead->y - 1;
        if (newLocation.y == 0)
            newLocation.y = HEIGHT;
        break;
    }

    if (newLocation.x == food->x && newLocation.y == food->y) {
        // eat food
        prependDoublyLinkedList(snakeList, &newLocation);
        food->x = -1;
        food->y = -1;
    } else {
        // move
        rotateTailWithNewData(snakeList, &newLocation);
    }

    return 0;
}

void display_snake(DoublyLinkedList *snakeList)
{
    DoublyNode *current = snakeList->head;
    while (current) {
        SnakeBody *body = (SnakeBody *)current->element;
        display_point(body->x, body->y, '*');
        current = current->next;
    }
}
void display_food(Food *food)
{
    if (food->x == -1)
        return;
    display_point(food->x, food->y, '#');
}

void display_outside(void)
{
    for (int i = 0; i < WIDTH; i++) {
        display_point(i, 0, '-');
        display_point(i, HEIGHT, '-');
    }

    for (int i = 0; i < HEIGHT; i++) {
        display_point(0, i, '|');
        display_point(WIDTH, i, '|');
    }
}

int main()
{
    srand((unsigned int)time(NULL));

    int birthPointx = rand() % WIDTH;
    int birthPointy = rand() % HEIGHT;
    int direction = calDirection(birthPointx, birthPointy);

    DoublyLinkedList *snakeBodyList = snakeInit(birthPointx, birthPointy);
    if (!snakeBodyList) {
        return -1;
    }

    display_init();

    int ch;
    Food food = {.x = -1, .y = -1};
    while (1) {
        ch = getch();

        switch (ch) {
        case KEY_UP:
        case 'w':
            direction = direction != DIRECTION_DOWN ? DIRECTION_UP : direction;
            break;
        case KEY_DOWN:
        case 's':
            direction = direction != DIRECTION_UP ? DIRECTION_DOWN : direction;
            break;
        case KEY_LEFT:
        case 'a':
            direction = direction != DIRECTION_RIGHT ? DIRECTION_LEFT : direction;
            break;
        case KEY_RIGHT:
        case 'd':
            direction = direction != DIRECTION_LEFT ? DIRECTION_RIGHT : direction;
            break;
        case 'q':
            return 0;
        case ERR:
            break;
        default:
            break;
        }
        generate_food(snakeBodyList, &food);
        moveSnake(snakeBodyList, &food, direction);
        display_clear();
        display_outside();
        display_food(&food);
        display_snake(snakeBodyList);
        usleep(200000);
    }

    display_deinit();
    snakeDeinit(snakeBodyList);

    return 0;
}
