#pragma once
#include "main.h"

typedef struct Level
{
	const char* StartDialogue;
	const char* EndDialogue;
} Level;

#define LEVEL_COUNT 1
extern Level Levels[LEVEL_COUNT];
extern Level *CurrentLevel;

void SetLevel(int index);
void UpdateLevel(float dt);