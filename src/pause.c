#include "pause.h"

bool IsGamePaused;

static float pauseFadeBG;

void PauseShow()
{
	ShowScreen("screen-pause");
	TweenManager_AddFloat(&pauseFadeBG, 0.8, 0.3, EASING_EASEINQUAD, "PauseBG", NULL);
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
	DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, (Color){0,0,0, 100 * pauseFadeBG});
	DrawText("GAME PAUSED", 4, 4, 30, WHITE);
}