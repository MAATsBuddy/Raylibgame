#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Olá, Raylib!");
    SetTargetFPS(60);
    float x = 400.0f, y = 300.0f;
    int size = 20;
    Color mosColor = BLACK;
    const float vel = 250.0f; /* px/s */
    bool placeDot = false;

    struct Ponto2D {
        int x;
        int y;
        int size;
        Color cor;
    };

    struct Ponto2D Tinta[42069];
    int lastDot = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) {
            x += vel * dt;
            placeDot = true;
        }
        if (IsKeyDown(KEY_LEFT)) {
            x -= vel * dt;
            placeDot = true;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += vel * dt;
            placeDot = true;
        }
        if (IsKeyDown(KEY_UP)) {
            y -= vel * dt;
            placeDot = true;
        }

        if (placeDot) {
            Tinta[lastDot].x = x;
            Tinta[lastDot].y = y;
            Tinta[lastDot].size = size;
            Tinta[lastDot].cor = mosColor;
            lastDot++;
        }
        placeDot = false;

        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle((int)x - size, (int)y - size, size, GREEN);
        DrawCircle((int)x - size + 2, (int)y - size + 2, size - 2, WHITE);
        DrawText(TextFormat("INK LEFT: %d/69420", lastDot), 300, 50, 20, BLACK);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
