#include "end.h"

static bool isWinState;

void GameEndShow(bool win)
{
	isWinState = win;
	ShowScreen("screen-end");

	// if (finishedGame)
	// {
	// 	ShowScreen("screen-end");
	// }
	// else if (isWinState)
	// {
	// 	ShowScreen("screen-end-win");
	// }
	// else
	// {
	// 	RetryCount++;
	// 	ShowScreen("screen-end-lose");
	// }
}

void GameEndUpdate(float dt)
{
	
}

void OnEndClick()
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

void GameEndRender(float dt)
{
	if (finishedGame)
	{
		
	}
	else if (isWinState)
	{
		
	}
	else
	{
		
	}
}

void GameEndHide()
{
}
