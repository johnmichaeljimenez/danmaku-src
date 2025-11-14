#include "pause.h"

bool IsGamePaused;

void PauseUpdate(float dt)
{
	if (IsKeyReleased(KEY_ESCAPE))
		UnpauseGame();
}

void PauseRender(float dt)
{
	DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, (Color){0,0,0, 100});
	DrawText("GAME PAUSED", 4, 4, 30, WHITE);
}