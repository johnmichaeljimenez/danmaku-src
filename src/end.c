#include "end.h"

static bool isWinState;

void GameEndShow(bool win)
{
	isWinState = win;
}

void GameEndUpdate(float dt)
{
	if (IsPointerDown() || IsKeyPressed(KEY_SPACE))
	{
		GameEndHide();
		if (isWinState)
		{
			GoToMenu();
		}
		else
		{
			RestartGame();
		}
	}
}

void GameEndRender(float dt)
{
	DrawText(isWinState ? "You win!\n[Hold] to exit" : "Game Over\n[Hold] to restart", 100, 300, 40, isWinState ? GREEN : RED);
}

void GameEndHide()
{
}
