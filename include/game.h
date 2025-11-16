#pragma once
#include "main.h"

extern bool IsCutscene;
extern int RetryCount;

void GameStart(int level);
void GameUpdate(float dt);
void GameRender(float dt);
void GameQuit();

void OnEndLevel(void);