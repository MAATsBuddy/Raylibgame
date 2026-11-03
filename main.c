/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{
	//rand
	srand(time(NULL));

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "LplusWulf");

	//initialize audio device
	InitAudioDevice();

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture Wulf = LoadTexture("WulfTemplate.png");

	// Load music from the resources directory
	Music music = LoadMusicStream("Train.mp3");

	//set fps
	SetTargetFPS(30);

	//variables
	Vector2 mousePos = { 100.0f, 100.0f };
	Color ballColor[4] = {DARKBLUE, DARKBLUE, DARKBLUE, DARKBLUE};
    float percentPlayed = 0.0f;

	//Conductor
	int bpm = 120;
	float crotchet;
	float songPos;
	float offset = 0.2f;
	int beat = 0;

	PlayMusicStream(music);
	SetMusicPan(music, 0.0f);
	SetMusicVolume(music, 1.0f);
	ResumeMusicStream(music);

	//song config (turn into func later)
	crotchet = 60.0/bpm;
	

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		//update
		if (IsKeyPressed(KEY_H))
        {
            if (IsCursorHidden()) ShowCursor();
            else HideCursor();
        }

		UpdateMusicStream(music);

		percentPlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
		if (percentPlayed > 1.0f) percentPlayed = 1.0f;



		//get mouse pos
		mousePos = GetMousePosition();

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLUE);

		//debug
		DrawText(TextFormat("test\n%.2f\n%.2f\n%.2f", crotchet, GetMusicTimeLength(music), GetMusicTimePlayed(music)),10,10,15,BLACK);

		// draw some text using the default font
		DrawText("Press H to hide/show mouse", 50,50,20,BLACK);

		// show time
		DrawText(TextFormat("%02.0f:%02d", GetMusicTimePlayed(music)/60, (int)GetMusicTimePlayed(music)%60), 650, 100, 20, BLACK);

		//playtime
		DrawRectangle(200, 100, 400, 12, LIGHTGRAY);
		DrawRectangle(200, 100, (int)(percentPlayed*400.0f), 12, MAROON);

		//draw circles
		for(int i=0;i<4;i++)
		{
			DrawCircle(200+(i*100), 400, 40, ballColor[i]);
		}

		// draw our texture to the screen
		DrawTextureV(Wulf, mousePos, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup

	// unload music
	UnloadMusicStream(music);  

	// (music streaming is automatically stopped)
	CloseAudioDevice();

	// unload our texture so it can be cleaned up
	UnloadTexture(Wulf);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
