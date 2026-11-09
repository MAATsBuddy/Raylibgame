#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Olá, Raylib!");
    SetTargetFPS(60);
    float x = 400.0f, y = 300.0f;
    int size = 20;
    Color mosColor = BLACK;
    const float vel = 250.0f; /* px/s */
    bool placeDot = false;
    bool mousePos = false;

    struct Ponto2D {
        int x;
        int y;
        int size;
        Color cor;
    };

    struct Ponto2D Tinta[50000];
    int lastDot = 0;
    int currentDot = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) {
            x += vel * dt;
        }
        if (IsKeyDown(KEY_LEFT)) {
            x -= vel * dt;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += vel * dt;
        }
        if (IsKeyDown(KEY_UP)) {
            y -= vel * dt;
        }
        if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            placeDot = true;
        }
        if (IsKeyDown(KEY_ONE)) {
            mosColor = BLACK;
        }
        if (IsKeyDown(KEY_TWO)) {
            mosColor = WHITE;
        }
        if (IsKeyDown(KEY_THREE)) {
            mosColor = BLUE;
        }
        if (IsKeyDown(KEY_FOUR)) {
            mosColor = RED;
        }
        if (IsKeyDown(KEY_FIVE)) {
            mosColor = GREEN;
        }
        if (IsKeyPressed(KEY_C)) {
            lastDot = 0;
        }
        if (IsKeyPressed(KEY_H)) {
            if (mousePos) {
                mousePos = false;
            } else {
                mousePos = true;
            }
        }
        if (mousePos) {
            x = GetMousePosition().x;
            y = GetMousePosition().y;
        }
        if (IsKeyDown(KEY_EQUAL)) {
            size++;
        }
        if (IsKeyDown(KEY_MINUS)) {
            size--;
            if (size <= 0) {
                size = 1;
            }
        }

        if (placeDot) {
            Tinta[lastDot].x = x;
            Tinta[lastDot].y = y;
            Tinta[lastDot].size = size;
            Tinta[lastDot].cor = mosColor;
            lastDot++;
        }
        placeDot = false;
        currentDot = 0;

        BeginDrawing();
        ClearBackground(WHITE);

        while (currentDot != lastDot) {
            DrawCircle(Tinta[currentDot].x, Tinta[currentDot].y,
                       Tinta[currentDot].size, Tinta[currentDot].cor);
            currentDot++;
        }

        DrawCircle((int)x, (int)y, size, mosColor);
        if (mosColor.r == WHITE.r || mosColor.b == WHITE.b ||
            mosColor.g == WHITE.g) {
            DrawCircle((int)x, (int)y, size - 2, BLACK);
        } else {
            DrawCircle((int)x, (int)y, size - 2, WHITE);
        }
        DrawText(TextFormat("INK LEFT: %d/50000", lastDot), 20, 20, 20, BLACK);
        DrawText("Seta para se mexer\nespaço/botão esquerdo para desenhar\n1-5 "
                 "para cores (2 apaga)\nC - clear screen\n H - troca entre "
                 "mouse ou teclado\n -/+ tamanho",
                 20, 40, 20, BLACK);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
