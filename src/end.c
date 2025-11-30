#include "end.h"

static bool isWinState;

void GameEndShow(bool win)
{
	isWinState = win;
}

void GameEndUpdate(float dt)
{
	if (IsPointerHold(dt) || IsInputPressed(KEY_SPACE))
	{
		GameEndHide();

		if (finishedGame)
		{
			GoToMenu();
		}
		else if (isWinState)
		{
			GoToGame(LevelIndex+1);
		}
		else
		{
			RetryCount++;
			RestartGame();
		}
	}
}

void GameEndRender(float dt)
{
	if (finishedGame)
	{
		DrawText("Congratulations! You win the game!\n[Hold] to exit", 100, 300, 40, GREEN);
	}
	else if (isWinState)
	{
		DrawText("You win!\n[Hold] to exit", 100, 300, 40, GREEN);
	}
	else
	{
		DrawText("Game Over\n[Hold] to restart", 100, 300, 40, RED);
	}
}

void GameEndHide()
{
}
