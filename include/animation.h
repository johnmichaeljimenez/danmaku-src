#pragma once
#include "main.h"

typedef struct SpriteEntry SpriteEntry;

typedef struct AnimationClip
{
	const char *ID;
	int FrameCount;
	const char **FrameIDs;
	Texture2D Frames[64];
	bool Loop;
	bool IsValid;
} AnimationClip;

typedef struct Animation
{
	float Timer;
	int FrameIndex;
	AnimationClip *Clip;
	bool IsValid;
} Animation;

#define ANIMATION_COUNT 1024
#define ANIMATIONCLIP_COUNT 2048
#define ANIMATION_FPS 1.0f / 30.0f

extern Animation Animations[ANIMATION_COUNT];
extern AnimationClip AnimationClips[ANIMATIONCLIP_COUNT];

void AddAnimationClip(const char *id, const char **frameIDs, int frameCount, bool loop);
Animation *CreateAnimation(const char *id);
void *ReuseAnimation(Animation *a, const char *id);
void RemoveAnimation(Animation *a);

void UpdateAnimations(float dt);
void ClearAnimations();
void SetupAnimationClips();