#include "levels.h"

const Level levelTutorial = {
	.StartDialogue = "tutorial-start",
	.EndDialogue = "tutorial-end"};

const Level level1 = {
	.StartDialogue = "",
	.EndDialogue = ""
};

const Level level2 = {
	.StartDialogue = "",
	.EndDialogue = ""
};

const Level level3 = {
	.StartDialogue = "",
	.EndDialogue = ""
};

const Level level4 = {
	.StartDialogue = "",
	.EndDialogue = ""
};

const Level level5 = {
	.StartDialogue = "",
	.EndDialogue = ""
};

Level Levels[LEVEL_COUNT] = {
	levelTutorial,
	level1, level2, level3, level4, level5
};

Level *CurrentLevel = &Levels[0];
float levelTimer;
int LevelIndex;

void OnLevelStart()
{
	const char* bossScript = 
	#ifdef TEST_MODE
		"level_test"
	#else
		LevelIndex == 0? "level_tutorial" : TextFormat("level_%d", LevelIndex);
	#endif

	RootBullet = SpawnBullet((Vector2){VIRTUAL_WIDTH * 0.5f, 0}, -90, false, "enemy_boss", bossScript);
	TraceLog(LOG_INFO, "Spawned boss %s", bossScript);

	RootBullet->IsRoot = true;
	RootBullet->IgnoreHit = true;
}

void SetLevel(int index)
{
	TraceLog(LOG_INFO, "Loaded level %d", index);
	LevelIndex = index;
	CurrentLevel = &Levels[index];
	levelTimer = 0;

	if (CurrentLevel->StartDialogue != NULL && CurrentLevel->StartDialogue != "")
	{
		if (index == 0)
		{
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
	else
	{
		OnLevelStart();
	}
}

void UpdateLevel(float dt)
{
	if (!player.IsAlive || IsCutscene || IsDialogueActive)
		return;

	levelTimer += dt;
}