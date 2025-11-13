#pragma once
#include "main.h"

typedef struct EnemyType EnemyType;

typedef struct Sequence
{
	float Timestamp;
	float Interval;
	int Count;

	Vector2 Position;
	float Direction;
	EnemyType *Type;

	bool IsDone;
} Sequence;

typedef struct Level
{
	int Count;
	Sequence *Items;
} Level;

#define LEVEL_COUNT 1
extern Level Levels[LEVEL_COUNT];
extern Level *CurrentLevel;

void SetLevel(int index);
void UpdateLevel(float dt);