#include "menu.h"

void MenuStart()
{
}

void MenuUpdate(float dt)
{
	if (IsKeyPressed(KEY_SPACE) || IsPointerDown())
		GoToGame(0);
}

void MenuRender(float dt)
{
	DrawText("Press [SPACE] to start", 30, VIRTUAL_HEIGHT * 0.4, 30, RED);
}

void MenuCleanup()
{
}
