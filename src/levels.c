#include "levels.h"

#define AddSequence(_ts, _intv, _amt, _posX, _posY, _dirX, _dirY, _type)           \
	(Sequence) { .Timestamp = (_ts), .Interval = (_intv), .Count = (_amt), \
				 .Position = (Vector2){.x = (_posX), .y = (_posY)},        \
				 .Direction = (Vector2){.x = (_dirX), .y = (_dirY)},       \
				 .e = (_type), .IsDone = false }

static const Sequence level0_items[] = {
	AddSequence(0.0f, 0.5f, 3, 100.0f, 50.0f, 1.0f, 0.0f, &ET_TEST),
	AddSequence(3.0f, 0.5f, 5, 300.0f, 200.0f, -1.0f, 0.0f, &ET_TEST),
	AddSequence(8.0f, 0.5f, 2, 150.0f, 400.0f, 0.0f, -1.0f, &ET_TEST),
};

const Level level0 = {
	.Count = arraySize(level0_items),
	.Items = (Sequence *)level0_items
};

Level Levels[LEVEL_COUNT] = {
	level0
};

Level* CurrentLevel = &Levels[0];
float levelTimer;
int spawnCount;

void SetLevel(int index)
{
	CurrentLevel = &Levels[index];
	levelTimer = 0;
	spawnCount = 0;
}

void UpdateLevel(float dt)
{
	levelTimer += dt;
	for (int i = 0; i < CurrentLevel->Count; i++)
	{
		Sequence *sq = &CurrentLevel->Items[i];
		if (sq->IsDone)
			continue;

		if (spawnCount >= sq->Count)
		{
			sq->IsDone = true;
		}
	}
}