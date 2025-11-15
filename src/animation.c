#include "animation.h"

Animation Animations[ANIMATION_COUNT];
AnimationClip AnimationClips[ANIMATIONCLIP_COUNT];

static int lastAnimationIndex;
static int lastAnimationClipIndex;

void AddAnimationClip(const char *id, Texture2D *frames, int frameCount, bool loop)
{
	AnimationClip *clip = &AnimationClips[lastAnimationClipIndex];
	clip->IsValid = true;
	clip->FrameCount = frameCount;
	clip->Frames = frames;
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

			if (a->FrameIndex >= a->Clip->FrameCount)
			{
				if (a->Clip->Loop)
					a->FrameIndex = 0;
			}
			else
			{
				a->FrameIndex++;
			}
		}
	}
}

void ClearAnimations()
{
	memset(Animations, 0, sizeof(Animations));
}