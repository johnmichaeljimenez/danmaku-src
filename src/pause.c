#include "pause.h"

bool IsGamePaused;

void PauseUpdate(float dt)
{
	if (IsKeyReleased(KEY_ESCAPE))
		UnpauseGame();
}

void PauseRender(float dt)
{
	DrawText("GAME PAUSED", 4, 4, 30, WHITE);
}