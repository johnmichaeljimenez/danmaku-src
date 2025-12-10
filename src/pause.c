#include "pause.h"

bool IsGamePaused;

void PauseShow()
{
	ShowScreen("screen-pause");
}

void PauseHide()
{
	ShowScreen("screen-game");
}

void PauseUpdate(float dt)
{
	if (IsKeyReleased(KEY_ESCAPE))
		UnpauseGame();
}

void PauseRender(float dt)
{
	DrawText("GAME PAUSED", 4, 4, 30, WHITE);
}