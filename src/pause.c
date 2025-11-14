#include "pause.h"

bool IsGamePaused;

void PauseUpdate()
{
	if (IsKeyReleased(KEY_ESCAPE))
		UnpauseGame();
}

void PauseRender()
{
	DrawText("GAME PAUSED", 4, 4, 30, WHITE);
}