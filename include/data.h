#pragma once

#include "main.h"
#define BULLET_COUNT 4096
#define BULLET_TYPE_COUNT 512
#define VFX_COUNT 2048

typedef struct Bullet Bullet;
typedef struct BulletScript BulletScript;
typedef struct Animation Animation;

typedef struct BulletType
{
	bool IsBoss;
	int HP;
	float Size;
	bool FixedRotation;

	const char *ID;
	const char *ScriptName;
	const char *AnimationName;
} BulletType;

typedef struct Bullet
{
	BulletType *Type;
	Animation *Animation;
	BulletScript *Script;

	int HP;
	Vector2 Position;
	Vector2 Velocity;
	float Size;
	float Angle;
	float WaitCounter;
	int SpawnCounter;
	float SpawnIntervalTimer;
	int OpIndex;

	bool IsRoot;
	bool FromPlayer;
	float Timer;
	bool IsAlive;
} Bullet;

typedef struct Player
{
	Animation *Animation;
	Vector2 Position;
	float HurtboxSize;

	float MovementSpeed;
	float FireRate;
	float FireTimer;

	int Lives;
	float ImmuneTime;
	bool IsAlive;
} Player;

typedef struct VFX
{
	int ID;
	Texture2D Sprite;
	Vector2 Position;
	float Direction;
	float Lifetime;
	float Scale;
	float Alpha;
	Color Tint;
	bool Additive;

	bool IsAlive;
	float Timer;
} VFX;

extern Bullet bullets[BULLET_COUNT];
extern VFX vfxPool[VFX_COUNT];
extern Player player;

extern Bullet* RootBullet;
extern BulletType bulletTypes[BULLET_TYPE_COUNT];

bool HitPlayer();

void DespawnBullet(Bullet *b);

Bullet *SpawnBullet(Vector2 pos, float angle, bool fromPlayer, const char *bulletType, const char *scriptOverride);
VFX *SpawnVFX(Vector2 pos, Texture2D sprite, float dir, float lifetime);