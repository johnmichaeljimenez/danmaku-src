#pragma once
#include "main.h"

typedef struct EnemyType EnemyType;
typedef void (*EnemyMovementFn)(Enemy *e, float dt);
typedef void (*EnemyAttackFn)(Enemy *e, float dt);

typedef struct Sequence
{
	float Timestamp;
	float Interval;
	int Count;

	Vector2 Position;
	Vector2 Target;
	EnemyType *Type;

	EnemyAttackFn MovementOverride;
	EnemyMovementFn AttackOverride;

	bool IsDone;
	int SpawnCount;
	float IntervalTimer;
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