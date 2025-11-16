#include "animation.h"

Animation Animations[ANIMATION_COUNT];
AnimationClip AnimationClips[ANIMATIONCLIP_COUNT];

static int lastAnimationIndex;
static int lastAnimationClipIndex;

void AddAnimationClip(const char *id, const char **frameIDs, int frameCount, bool loop, int loopFrame)
{
	AnimationClip *clip = &AnimationClips[lastAnimationClipIndex];
	clip->IsValid = true;
	clip->FrameCount = frameCount;
	clip->LoopFrame = loopFrame;

	for (int i = 0; i < frameCount; i++)
	{
		clip->Frames[i] = GetSprite(frameIDs[i]);
	}

	clip->FrameIDs = frameIDs;
	clip->ID = id;
	clip->Loop = loop;

	lastAnimationClipIndex++;
}

Animation *CreateAnimation(const char *id)
{
	Animation *a = &Animations[lastAnimationIndex];
	a->IsValid = true;
	a->Timer = 0;
	a->FrameIndex = 0;

	for (int i = 0; i < ANIMATIONCLIP_COUNT; i++)
	{
		AnimationClip *c = &AnimationClips[i];
		if (c->IsValid && TextIsEqual(id, c->ID))
		{
			a->Clip = c;
			break;
		}
	}

	lastAnimationIndex++;
	// TraceLog(LOG_INFO, "CREATED ANIMATION: %s FROM CLIP %s %d %d x %d", id, a->Clip->ID, a->Clip->FrameCount, a->Frames[0].width, a->Frames[0].height);
	return a;
}

void *ReuseAnimation(Animation* a, const char* id)
{
	a->Timer = 0;
	a->FrameIndex = 0;

	for (int i = 0; i < ANIMATIONCLIP_COUNT; i++)
	{
		AnimationClip *c = &AnimationClips[i];
		if (c->IsValid && TextIsEqual(id, c->ID))
		{
			a->Clip = c;
			break;
		}
	}
}

void RemoveAnimation(Animation *a)
{
	a->IsValid = false;
}

void UpdateAnimations(float dt)
{
	for (int i = 0; i < ANIMATION_COUNT; i++)
	{
		Animation *a = &Animations[i];
		if (!a->IsValid)
			continue;

		a->Timer += dt;

		if (a->Timer >= ANIMATION_FPS)
		{
			a->Timer = 0;
			a->FrameIndex++;

			if (a->FrameIndex >= a->Clip->FrameCount)
			{
				if (a->Clip->Loop)
					a->FrameIndex = a->Clip->LoopFrame;
				else
					a->FrameIndex = a->Clip->FrameCount - 1;
			}
		}
	}
}

static const char *playerIdle[] = {
	"character/character-idle (1)",
	"character/character-idle (2)",
	"character/character-idle (3)",
	"character/character-idle (4)",
	"character/character-idle (5)",
	"character/character-idle (6)",
	"character/character-idle (7)",
	"character/character-idle (8)"};

static const char *playerLeft[] = {
	"character/character-left (1)",
	"character/character-left (2)",
	"character/character-left (3)",
	"character/character-left (4)",
	"character/character-left (5)",
	"character/character-left (6)",
	"character/character-left (7)",
	"character/character-left (8)"};

static const char *playerRight[] = {
	"character/character-right (1)",
	"character/character-right (2)",
	"character/character-right (3)",
	"character/character-right (4)",
	"character/character-right (5)",
	"character/character-right (6)",
	"character/character-right (7)",
	"character/character-right (8)"};
	

static const char *enemyGeneric[] = {
	"enemy/enemy-generic (1)",
	"enemy/enemy-generic (2)",
	"enemy/enemy-generic (3)",
	"enemy/enemy-generic (4)",};

void SetupAnimationClips()
{
	AddAnimationClip("PlayerIdle", playerIdle, 8, true, 0);
	AddAnimationClip("PlayerLeft", playerLeft, 8, true, 3);
	AddAnimationClip("PlayerRight", playerRight, 8, true, 3);
	AddAnimationClip("EnemyGeneric", enemyGeneric, 4, true, 0);
}