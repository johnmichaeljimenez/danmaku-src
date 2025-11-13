#include "levels.h"

#define AddSequence(_ts, _intv, _amt, _posX, _posY, _dir, _type)   \
	(Sequence) { .Timestamp = (_ts), .Interval = (_intv), .Count = (_amt), \
				 .Position = (Vector2){.x = (_posX), .y = (_posY)},        \
				 .Direction = _dir,       \
				 .Type = (_type), .IsDone = false }

static const Sequence level0_items[] = {
	AddSequence(0.0f, 0.5f, 3, VIRTUAL_WIDTH * 0.5, 50.0f, -90, &ET_TEST),
	AddSequence(10.0f, 0.5f, 5, VIRTUAL_WIDTH * 0.5, 50.0f, -90, &ET_TEST),
	AddSequence(20.0f, 0.5f, 2, VIRTUAL_WIDTH * 0.5, 50.0f, -90, &ET_TEST),
};

const Level level0 = {
	.Count = arraySize(level0_items),
	.Items = (Sequence *)level0_items};

Level Levels[LEVEL_COUNT] = {
	level0};

Level *CurrentLevel = &Levels[0];
float levelTimer;
float intervalTimer;
int spawnCount;

void SetLevel(int index)
{
	CurrentLevel = &Levels[index];
	levelTimer = 0;
	spawnCount = 0;
	intervalTimer = 0;
}

void UpdateLevel(float dt)
{
	levelTimer += dt;
	for (int i = 0; i < CurrentLevel->Count; i++)
	{
		Sequence *sq = &CurrentLevel->Items[i];
		if (sq->IsDone)
			continue;

		if (sq->Timestamp > levelTimer)
			continue;

		if (sq->Interval > intervalTimer)
		{
			intervalTimer += dt;
		}
		else
		{
			intervalTimer = 0;
			SpawnEnemy(sq->Position, sq->Direction, sq->Type);
			spawnCount++;
		}

		if (spawnCount >= sq->Count)
		{
			sq->IsDone = true;
			spawnCount = 0;
			intervalTimer = 0;
		}
	}
}