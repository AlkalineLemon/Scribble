#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

//Global Variables
int screenWidth = 1280;
int screenHeight = 720;

bool show = false;

//Prototypes
void standardToolbarDraw(float ,float,float,float);


int main() {

	InitWindow(screenWidth, screenHeight, "Scribble");
	GuiLoadStyle("dark.rgs");

	while(!WindowShouldClose())
	{

		//Drawing
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
		
		//GUI ELEMENTS START
		standardToolbarDraw(0, 0, 0.1f, 0.05f);
		//GUI ELEMENTS END


		EndDrawing();
	}
	CloseWindow();
	return 0;
}

//The widths and heights are relative to screen dimensions
void standardToolbarDraw(float x, float y, float w, float h)
{
	w *= screenWidth;
	h *= screenHeight;
	int fileIndex = 0;

	Rectangle r = { x, y, w, h };
	if (GuiDropdownBox(r, "File;New;Open;Save;Save As;Close", &fileIndex, true))
	{
		show = true;
	}
	else show = false;


	
}