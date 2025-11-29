#include "start.h"

bool started;

void StartInit()
{

}

void StartUpdate(float dt)
{
	if (IsPointerPressed())
	{
		started = true;
		InitAudio();
		MenuStart();
	}
}

void StartRender(float dt)
{
	DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, BLACK);
	DrawTextEx(DefaultFont, "Press to [Start]", (Vector2){4, 500}, 24, 0, WHITE);
}