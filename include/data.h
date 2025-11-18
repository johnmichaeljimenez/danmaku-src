#pragma once

#include "main.h"
#define BULLET_COUNT 4096
#define VFX_COUNT 2048

typedef struct Bullet Bullet;
typedef struct BulletScript BulletScript;
typedef struct Animation Animation;

typedef struct BulletType
{
	bool IsBoss;
	int HP;
	float MovementSpeed;
	float Size;

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
	int OpIndex;

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

extern BulletType BT_PLAYER;
// extern EnemyType ET_TEST;
// extern EnemyType ET_BOSS1;

bool HitPlayer();

void DespawnBullet(Bullet *b);

Bullet *SpawnBullet(Vector2 pos, float angle, bool fromPlayer, BulletType *bulletType, const char *scriptOverride);
VFX *SpawnVFX(Vector2 pos, Texture2D sprite, float dir, float lifetime);