#include "raylib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

int main() {
    // rand
    srand(time(NULL));

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(800, 600, "SheepJump");

    float x = 50.0f;
    float y = 470.0f;
    float vx = 2.0f;
    float vy = 0.0f;
    float smokeframe = 0.0f;

    // game loop
    while (!WindowShouldClose()) // run the loop until the user presses ESCAPE
    {

        // drawing
        BeginDrawing();

        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(DARKGRAY);

        x = x + vx;
        y = y + vy;

        // sheep jump
        if (x > 300) {
            vy = -10;
        }
        if (y < 470) {
            vy = vy - 0.5f;
        }
        if (y < -150) {
            y = 470;
            vy = 0;
            x = -50;
            smokeframe = 0.0f;
        }

        // ground
        DrawRectangle(0, 500, 800, 100, DARKGREEN);

        // fence
        DrawLineEx((Vector2){400, 490}, (Vector2){400, 450}, 10.0, BROWN);
        DrawLineEx((Vector2){400, 490}, (Vector2){400, 540}, 10.0, DARKBROWN);

        // sheep body
        DrawCircle(x + 6, y + 2, 20, WHITE);
        DrawCircle(x - 8, y - 5, 20, WHITE);
        DrawCircle(x + 8, y - 14, 18, WHITE);
        DrawCircle(x - 6, y + 3, 20, WHITE);
        DrawCircle(x - 19, y - 6, 18, WHITE);
        DrawCircle(x - 25, y + 3, 16, WHITE);
        DrawEllipse(x + 30, y - 20, 12.0f, 8.0f, BLACK);

        DrawTriangle((Vector2){x - 20, y + 20}, (Vector2){x - 30, y + 15},
                     (Vector2){x - 32, y + 45}, BLACK);

        DrawTriangle((Vector2){x + 18, y + 15}, (Vector2){x + 8, y + 20},
                     (Vector2){x + 22, y + 45}, BLACK);

        if (x > 300) {
            smokeframe = smokeframe + 1;
            DrawCircle(300 + smokeframe * 2, 490, 20 - smokeframe, GRAY);
            DrawCircle(300 - smokeframe * 2, 490, 20 - smokeframe, GRAY);
            DrawCircle(300 + smokeframe, 500, 20 - smokeframe * 0.5f, GRAY);
            DrawCircle(300 - smokeframe, 500, 20 - smokeframe * 0.5f, GRAY);
        }

        // // rainBack
        // for (int i = 0; i < 10; i++) {
        //     int randx = rand() % 790 + 10;
        //     int randy = rand() % 620 - 10;
        //     DrawLineEx((Vector2){randx, randy}, (Vector2){randx, randy + 40},
        //                2.0, SKYBLUE);
        // }

        // // tail
        // DrawEllipse(300, 600, 90.0f, 80.0f, WHITE);
        // DrawEllipse(240, 570, 50.0f, 30.0f, WHITE);
        // DrawEllipse(180, 600, 80.0f, 50.0f, WHITE);
        // // leg
        // DrawCircle(470, 600, 70, WHITE);
        // DrawCircle(470, 600, 55, BLUE);
        // DrawCircle(542, 598, 12, WHITE);
        // // body
        // DrawEllipse(400, 550, 70.0f, 250.0f, WHITE);
        // DrawRectangle(330, 350, 140, 200, WHITE);
        // DrawEllipse(400, 350, 70.0f, 50.0f, WHITE);
        // DrawEllipse(360, 320, 30.0f, 100.0f, WHITE);
        // DrawEllipse(440, 320, 30.0f, 100.0f, WHITE);
        // // face
        // DrawTriangle((Vector2){450, 400}, (Vector2){420, 400},
        //              (Vector2){435, 425}, BLACK);
        // DrawRectangle(443, 350, 12, 33, BLACK);
        // DrawRectangle(403, 350, 12, 33, BLACK);
        // // arm
        // DrawRectangle(465, 450, 120, 20, WHITE);
        // // spear
        // DrawRectangle(580, 300, 15, 350, BLACK);
        // DrawTriangle((Vector2){588, 210}, (Vector2){580, 300},
        //              (Vector2){595, 300}, BLACK);
        // // hand
        // DrawCircle(595, 460, 10, WHITE);
        // DrawRectangle(598, 450, 20, 20, BLUE);

        // // rainFront
        // for (int i = 0; i < 10; i++) {
        //     int randx = rand() % 790 + 10;
        //     int randy = rand() % 620 - 10;
        //     DrawLineEx((Vector2){randx, randy}, (Vector2){randx, randy + 40},
        //                2.0, SKYBLUE);
        // }
        // int splash = rand() % 6;
        // for (int i = 0; i < splash; i++) {
        //     int randx = rand() % 790 + 10;
        //     DrawLineEx((Vector2){randx - 5, 595}, (Vector2){randx - 20, 585},
        //                2.0, SKYBLUE);
        //     DrawLineEx((Vector2){randx + 5, 595}, (Vector2){randx + 20, 585},
        //                2.0, SKYBLUE);
        // }

        // end the frame and get ready for the next one  (display frame, poll
        // input, etc...)
        EndDrawing();
    }

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}