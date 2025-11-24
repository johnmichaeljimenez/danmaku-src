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
	if (lastAnimationIndex >= ANIMATION_COUNT)
		lastAnimationIndex = 0;
		
	// TraceLog(LOG_INFO, "CREATED ANIMATION: %s FROM CLIP %s %d %d x %d", id, a->Clip->ID, a->Clip->FrameCount, a->Frames[0].width, a->Frames[0].height);
	return a;
}

void *ReuseAnimation(Animation *a, const char *id)
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
static const char *playerBullet[] = {
	SPRITE_BULLET_DEFAULT
};

static const char *playerIdle[] = {
	SPRITE_CHARACTER_IDLE_1,
	SPRITE_CHARACTER_IDLE_2,
	SPRITE_CHARACTER_IDLE_3,
	SPRITE_CHARACTER_IDLE_4,
	SPRITE_CHARACTER_IDLE_5,
	SPRITE_CHARACTER_IDLE_6,
	SPRITE_CHARACTER_IDLE_7,
	SPRITE_CHARACTER_IDLE_8,
};

static const char *playerLeft[] = {
	SPRITE_CHARACTER_LEFT_1,
	SPRITE_CHARACTER_LEFT_2,
	SPRITE_CHARACTER_LEFT_3,
	SPRITE_CHARACTER_LEFT_4,
	SPRITE_CHARACTER_LEFT_5,
	SPRITE_CHARACTER_LEFT_6,
	SPRITE_CHARACTER_LEFT_7,
	SPRITE_CHARACTER_LEFT_8,
};

static const char *playerRight[] = {
	SPRITE_CHARACTER_RIGHT_1,
	SPRITE_CHARACTER_RIGHT_2,
	SPRITE_CHARACTER_RIGHT_3,
	SPRITE_CHARACTER_RIGHT_4,
	SPRITE_CHARACTER_RIGHT_5,
	SPRITE_CHARACTER_RIGHT_6,
	SPRITE_CHARACTER_RIGHT_7,
	SPRITE_CHARACTER_RIGHT_8,
};

static const char *enemyGeneric[] = {
	SPRITE_ENEMY_GENERIC_1,
	SPRITE_ENEMY_GENERIC_2,
	SPRITE_ENEMY_GENERIC_3,
	SPRITE_ENEMY_GENERIC_4
};

void SetupAnimationClips()
{
	AddAnimationClip("PlayerBulletDefault", playerBullet, 1, true, 0);
	AddAnimationClip("PlayerIdle", playerIdle, 8, true, 0);
	AddAnimationClip("PlayerLeft", playerLeft, 8, true, 3);
	AddAnimationClip("PlayerRight", playerRight, 8, true, 3);
	AddAnimationClip("EnemyGeneric", enemyGeneric, 4, true, 0);
}