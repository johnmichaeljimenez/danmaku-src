#pragma once
#include "main.h"

typedef struct Level
{
	const char* StartDialogue;
	const char* EndDialogue;
} Level;

#define LEVEL_COUNT 6
extern Level Levels[LEVEL_COUNT];
extern Level *CurrentLevel;
extern int LevelIndex;

void SetLevel(int index);
void UpdateLevel(float dt);