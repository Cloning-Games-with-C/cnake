#include "stdio.h"
#include "stdbool.h"

#include "raylib.h"

#define CELL_SIZE 20
#define WIDTH 800
#define HEIGHT 800

typedef struct
{
    int x;
    int y;
} Point;

Point food = {0};
Point snake[WIDTH * HEIGHT] = {0};
int len = 1;

int dx;
int dy;

int score;
int hiscore;
bool gameover;

void init_snake()
{
    snake[0] = (Point){.x = 10 * 10, .y = 10 * 10};

    for (int i = 1; i < len; i++)
    {
        snake[i] = (Point){.x = -1, .y = -1};
    }
}

void update_direction()
{
    if (IsKeyReleased(KEY_A) && dx == 0)
    {
        dx = -1;
        dy = 0;
    }
    else if (IsKeyReleased(KEY_D) && dx == 0)
    {
        dx = 1;
        dy = 0;
    }
    else if (IsKeyReleased(KEY_W) && dy == 0)
    {
        dy = -1;
        dx = 0;
    }
    else if (IsKeyReleased(KEY_S) && dy == 0)
    {
        dx = 0;
        dy = 1;
    }
}

void gen_food()
{
    int x = GetRandomValue(0, (WIDTH / CELL_SIZE) - CELL_SIZE);
    int y = GetRandomValue(0, (HEIGHT / CELL_SIZE) - CELL_SIZE);

    food.x = x * CELL_SIZE;
    food.y = y * CELL_SIZE;

    if (food.y < 40)
    {
        gen_food();
    }
}

void update_snake()
{
    if (snake[0].x == food.x && snake[0].y == food.y)
    {
        gen_food();
        len++;
        score++;
    }

    for (size_t i = len; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }

    snake[0].x += dx * CELL_SIZE;
    snake[0].y += dy * CELL_SIZE;

    if (snake[0].x < 0)
    {
        snake[0].x = WIDTH - 20;
    }
    else if (snake[0].x >= WIDTH)
    {
        snake[0].x = 0;
    }
    if (snake[0].y < 40)
    {
        snake[0].y = HEIGHT - 20;
    }
    else if (snake[0].y >= HEIGHT)
    {
        snake[0].y = 40;
    }
}

void check_death()
{
    if (len > 1)
    {
        for (int i = 1; i < len; i++)
        {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                gameover = true;
                return;
            }
        }
    }
}

void try_restart()
{
    if (IsKeyReleased(KEY_SPACE))
    {
        gameover = false;
        hiscore = score;
        score = 0;

        gen_food();
        init_snake();
        len = 1;
        dx = 0;
        dy = 0;
    }
}

int main(void)
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, "cnake");
    SetTargetFPS(10);
    SetTraceLogLevel(LOG_DEBUG);

    init_snake();
    gen_food();

    while (!WindowShouldClose())
    {
        try_restart();

        if (!gameover)
        {
            update_direction();
            update_snake();
            check_death();
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawRectangle(0, 0, WIDTH, 40, WHITE);
        DrawRectangle(0, 40, WIDTH, 1, BLACK);

        DrawRectangle(food.x, food.y, CELL_SIZE, CELL_SIZE, GREEN);

        DrawRectangle(snake[0].x, snake[0].y, CELL_SIZE, CELL_SIZE, BLUE);
        for (int i = 1; i < len; i++)
        {
            DrawRectangle(snake[i].x, snake[i].y, CELL_SIZE, CELL_SIZE, RED);
        }

        char score_text[18];
        snprintf(score_text, 18, "Score: %06d", score);
        DrawText(score_text, 10, 10, 22, BLACK);

        char hiscore_text[20];
        snprintf(hiscore_text, 18, "HiScore: %06d", hiscore);
        DrawText(hiscore_text, 10 + MeasureText(score_text, 22) + 10, 10, 22, BLACK);

        if (gameover)
        {
            DrawText("Game Over!", (WIDTH >> 1) - (MeasureText("Game Over!", 22) >> 1), HEIGHT >> 1, 22, WHITE);
            DrawText(score_text, (WIDTH >> 1) - (MeasureText(score_text, 22) >> 1), (HEIGHT >> 1) + 40, 22, WHITE);
            DrawText("Press SPACE to restart!", (WIDTH >> 1) - (MeasureText("Press SPACE to restart!", 22) >> 1), (HEIGHT >> 1) + 80, 22, WHITE);
        }

        EndDrawing();
    }

    SetTraceLogLevel(LOG_NONE);

    CloseWindow();
}