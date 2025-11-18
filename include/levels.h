#pragma once
#include "main.h"

typedef struct Sequence
{
	float Timestamp;
	float Interval;
	int Count;

	Vector2 Position;
	Vector2 Target;

	
	bool IsDone;
	int SpawnCount;
	float IntervalTimer;
} Sequence;

typedef struct Level
{
	int Count;
	Sequence *Items;
	const char* StartDialogue;
	const char* EndDialogue;
} Level;

#define LEVEL_COUNT 1
extern Level Levels[LEVEL_COUNT];
extern Level *CurrentLevel;

void SetLevel(int index);
void UpdateLevel(float dt);