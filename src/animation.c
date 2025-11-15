#include "animation.h"

Animation Animations[ANIMATION_COUNT];
AnimationClip AnimationClips[ANIMATIONCLIP_COUNT];

static int lastAnimationIndex;
static int lastAnimationClipIndex;

void AddAnimationClip(const char *id, const char **frameIDs, int frameCount, bool loop)
{
	AnimationClip *clip = &AnimationClips[lastAnimationClipIndex];
	clip->IsValid = true;
	clip->FrameCount = frameCount;

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
					a->FrameIndex = 0;
				else
					a->FrameIndex = a->Clip->FrameCount - 1;
			}
		}
	}
}

void ClearAnimations()
{
	memset(Animations, 0, sizeof(Animations));
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

void SetupAnimationClips()
{
	AddAnimationClip("PlayerIdle", playerIdle, 8, true);
}