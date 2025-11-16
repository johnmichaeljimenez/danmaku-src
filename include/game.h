#pragma once
#include "main.h"

extern bool IsCutscene;

void GameStart(int level);
void GameUpdate(float dt);
void GameRender(float dt);
void GameQuit();

void OnEndLevel(const char *id);