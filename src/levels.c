#include "levels.h"

#define AddSequence(_ts, _intv, _amt, _posX, _posY, _toX, _toY, _type, _mv, _atk) \
	(Sequence) { .Timestamp = (_ts), .Interval = (_intv), .Count = (_amt),              \
				 .Position = (Vector2){.x = (_posX), .y = (_posY)},                     \
				 .Target = (Vector2){.x = (_toX), .y = (_toY)},                         \
				 .Type = (_type), .IsDone = false, .AttackOverride = _atk, .MovementOverride = _mv }

static const Sequence level0_items[] = {
	AddSequence(0.0f, 0.5f, 3, VIRTUAL_WIDTH * 0.5, 0, VIRTUAL_WIDTH * 0.5, VIRTUAL_HEIGHT, &ET_TEST, NULL, NULL),
	AddSequence(5.0f, 0.5f, 5, VIRTUAL_WIDTH * 0.2, 0, VIRTUAL_WIDTH * 0.2, VIRTUAL_HEIGHT, &ET_TEST, NULL, NULL),
	AddSequence(8.0f, 0.5f, 2, VIRTUAL_WIDTH * 0.8, 0, VIRTUAL_WIDTH * 0.8, VIRTUAL_HEIGHT, &ET_TEST, NULL, NULL),
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

	for (int i = 0; i < LEVEL_COUNT; i++)
	{
		Level *lvl = &Levels[i];
		for (int j = 0; j < lvl->Count; j++)
		{
			Sequence *sq = &lvl->Items[j];
			sq->IsDone = false;
		}
	}
}

void UpdateLevel(float dt)
{
	if (!player.IsAlive || IsCutscene)
		return;

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
			SpawnEnemy(sq->Position, sq->Target, sq->Type);
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