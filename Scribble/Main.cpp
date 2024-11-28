#include "raylib.h"
#include <cstring>
#include<string>
#include<cmath>

//A header file with an array-based implementation of stack
#include "stack.h"


#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

//A struct that stores all the info for a rectangle in itseld
struct Rect
{
	Rect(Vector2 initial, Vector2 final, Color c)
	{
		width = abs(initial.x - final.x);
		height = abs(initial.y - final.y);
		//From 2 points, we can extrapolate everything for a rectangle
		x = (initial.x - final.x) >= 0 ? initial.x - width : initial.x;
		y = (initial.y - final.y) >= 0 ? initial.y - height : initial.y;
		color = c;

	}
	Rect()
	{
		width = 0;
		height = 0;
		x = 0;
		y = 0;
		color = WHITE;
	}
	Color color;

	int x;
	int y;
	int width;
	int height;
};

int initialCanvasWidth = 600;
int initialCanvasHeight = 600;

int screenWidth = 1280;
int screenHeight = 720;

//Prototypes:
//This function draws a boundry over the screen, the parameters are %s of screen width/height
void drawBorders(float left, float right, float top, float bottom, Color color);

int main()
{
	//Create a window
	InitWindow(screenWidth, screenHeight, "Scribble");
	//Maximize it and then make it non-resizable
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	MaximizeWindow();
	ClearWindowState(FLAG_WINDOW_RESIZABLE);
	//Store the width and height of the maximized window
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();

	GuiLoadStyle("dark.rgs");
	SetTargetFPS(60);

	float posX = 100;
	float posY = 100;

	Camera2D camera = { 0 };
	//NOTE: This syntax is only usable for initalization
	camera.target = { posX, posY };
	camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
	//camera.offset = { 0,0 };
	camera.zoom = 1.0f;
	camera.rotation = 0;

	//A basic undo-redo example with stacks;
	Stack<Rect> undo(10);
	Stack<Rect> redo(10);

	Vector2 initialPos;
	Vector2 finalPos;

	//We create a white image file, perform changes in it and then convert it to a texture for rendering
	Image image = GenImageColor(initialCanvasWidth, initialCanvasHeight, WHITE);


	Texture2D tex = LoadTextureFromImage(image);



	while (!WindowShouldClose())
	{
		//Basic movement
		if (IsKeyDown(KEY_A)) posX -= 3;
		if (IsKeyDown(KEY_D)) posX += 3;
		if (IsKeyDown(KEY_W)) posY -= 3;
		if (IsKeyDown(KEY_S)) posY += 3;

		//UNDO-REDO
		//If there are actions on the canvas and the hotkey is pressed, we undo it
		int key = GetKeyPressed();
		if (!undo.isEmpty() && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z))
		{
			redo.push(undo.topElement());

			undo.pop();
		}
		if (!redo.isEmpty() && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Y))
		{
			undo.push(redo.topElement());

			redo.pop();
		}

		//Camera-related stuff
		//Scroll-wheel based zooming
		camera.zoom += GetMouseWheelMove() * 0.25f;
		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
		if (camera.zoom < 0.25f) camera.zoom = 0.25f;

		//Mouse-Drag based movement
		
		if (IsKeyDown(KEY_L))
		{
			//Add rectangles to the undo stack using mouse dragging
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				//Save the initial position of a mouse press
				initialPos = GetScreenToWorld2D(GetMousePosition(), camera);

			}
			//CREATING RECTANGLES
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{

				//Save the final position
				finalPos = GetScreenToWorld2D(GetMousePosition(), camera);

				if (initialPos.x != finalPos.x && initialPos.y != finalPos.y)
				{
					//Push the rectangle made by the points into the undo stack
					undo.push({ initialPos , finalPos , YELLOW });
				}

			}
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			posX += GetMouseDelta().x;
			posY += GetMouseDelta().y;
		}																														
		camera.target.x = posX;
		camera.target.y = posY;



		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		BeginMode2D(camera);


		//Draw everything in the undo stack
		for (int i = 0;i <= undo.currentIndex;i++)
		{
			DrawRectangle(undo.items[i].x, undo.items[i].y, undo.items[i].width, undo.items[i].height, undo.items[i].color);
		}

		//Draw the canvas
		DrawTexture(tex, -200, -200, WHITE);

		//A red rect that marks the center of the camera
		DrawRectangle(posX, posY, 10, 10, RED);



		EndMode2D();

		//Drawing borders all over the screen
		drawBorders(0.1, 0.05, 0.075, 0.025, { 16,16,16,255 });

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void drawBorders(float left, float right, float top, float bottom, Color color)
{
	//We can draw 4 rectangles to get the same effect
	DrawRectangle(0, 0, screenWidth, screenHeight * top, color); //Top
	DrawRectangle(0, screenHeight * (1 - bottom), screenWidth, screenHeight * bottom, color); //Bottom
	DrawRectangle(0, 0, screenWidth * left, screenHeight, color); //Left
	DrawRectangle(screenWidth * (1 - right), 0, screenWidth * right, screenHeight, color); //Right

}
