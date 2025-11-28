#pragma once
#include "main.h"

extern bool IsCutscene;
extern int RetryCount;
extern bool finishedGame;

void GameStart(int level);
void GameUpdate(float dt);
void GameRender(float dt);
void GameQuit();

void SetHitstop(float duration);

void OnEndLevel(void);