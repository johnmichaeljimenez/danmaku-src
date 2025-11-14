#pragma once
#include "main.h"

extern bool IsGamePaused;

void PauseShow();
void PauseHide();
void PauseUpdate(float dt);
void PauseRender(float dt);