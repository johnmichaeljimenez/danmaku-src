#include "levels.h"

const Level level0 = {
	.StartDialogue = "tutorial-start",
	.EndDialogue = "tutorial-end"};

Level Levels[LEVEL_COUNT] = {
	level0
};

Level *CurrentLevel = &Levels[0];
float levelTimer;
int currentLevelIndex;

void OnLevelStart()
{
	RootBullet = SpawnBullet((Vector2){VIRTUAL_WIDTH * 0.5f, 0}, -90, false, "enemy_boss", TextFormat("level_%d", currentLevelIndex+1));
	RootBullet->IsRoot = true;
	RootBullet->IgnoreHit = true;
}

void SetLevel(int index)
{
	currentLevelIndex = index;
	CurrentLevel = &Levels[index];
	levelTimer = 0;

	if (CurrentLevel->StartDialogue != NULL)
	{
		if (index == 0)
		{
			// TraceLog(LOG_INFO, "%d", RetryCount);
			if (RetryCount > 0)
			{
				if (RetryCount >= 3)
					return;

				DialogueShow(TextFormat("tutorial-retry-%d", RetryCount), OnLevelStart);
				return;
			}
		}

		DialogueShow(CurrentLevel->StartDialogue, OnLevelStart);
	}
}

void UpdateLevel(float dt)
{
	if (!player.IsAlive || IsCutscene || IsDialogueActive)
		return;

	levelTimer += dt;
}