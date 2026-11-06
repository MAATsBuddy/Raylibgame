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
	float dt = GetFrameTime();

	//Conductor
	int bpm = 150;
	float crotchet;
	float songPos;
	float offset = 0.5f;
	int beat = 0;
	int lastBeat = 0;
	bool nextBeat = false;

	int response = 0;
	float inputTimer = 0;

	PlayMusicStream(music);
	SetMusicPan(music, 0.0f);
	SetMusicVolume(music, 1.0f);
	ResumeMusicStream(music);
	
	//config conductor
	crotchet = 60/(float)bpm;

	// game loop
	while (!WindowShouldClose())
	{
		//update -----------------------------------
		dt = GetFrameTime();

		if (inputTimer > 0) inputTimer -= dt;

		if (IsKeyPressed(KEY_H))
        {
            if (IsCursorHidden()) ShowCursor();
            else HideCursor();
        }

		UpdateMusicStream(music);

		percentPlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
		if (percentPlayed > 1.0f) percentPlayed = 1.0f;

		songPos = GetMusicTimePlayed(music) + offset;
		beat = (int)(songPos/crotchet);

		
		if(lastBeat != beat){lastBeat = beat; nextBeat = true;}
		else {nextBeat = false;}

		//get mouse pos
		mousePos = GetMousePosition();

		//input
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(beat%8 == 7) {response = 1; inputTimer = crotchet*2;}
			else {response = 2; inputTimer = crotchet;}
		}
		if(beat%8 == 0 && response != 1) {response = 2; inputTimer = crotchet;}
		if(inputTimer < 0){
			inputTimer = 0;
			response = 0;
		}

		// drawing --------------------------------
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLUE);

		//debug
		DrawText(TextFormat("test\ncrochet: %.2f\nlength: %.2f\nplayed: %.2f\nbeat: %d\ninput: %.2f"
							, crotchet, GetMusicTimeLength(music), GetMusicTimePlayed(music), beat, inputTimer),10,10,15,BLACK);

		// draw some text using the default font
		DrawText("Press H to hide/show mouse", 300,50,20,BLACK);

		// show time
		DrawText(TextFormat("%02.0f:%02d", GetMusicTimePlayed(music)/60, (int)GetMusicTimePlayed(music)%60), 650, 100, 20, BLACK);

		//playtime
		DrawRectangle(200, 100, 400, 12, LIGHTGRAY);
		DrawRectangle(200, 100, (int)(percentPlayed*400.0f), 12, MAROON);

		//draw circles
		for(int i=1;i<4;i++)
		{	
			if(beat%8 == i*2 || beat%8 == i*2-1) 
				DrawCircle(150+(i*100), 400, 40, ORANGE);
			else DrawCircle(150+(i*100), 400, 40, DARKBLUE);
			
		}
		switch (response)
		{
			case 0:
				DrawCircle(550, 400, 40, DARKBLUE);
				break;
			case 1:
				DrawCircle(550, 400, 40, YELLOW);
				break;
			case 2:
				DrawCircle(550, 400, 40, RED);
				break;
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
