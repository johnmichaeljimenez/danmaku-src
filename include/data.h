#pragma once

#include "main.h"
#define BULLET_COUNT 2048
#define ENEMY_COUNT 64
#define VFX_COUNT 2048

typedef struct Enemy Enemy;
typedef struct Bullet Bullet;
typedef void (*EnemyMovementFn)(Enemy *e, float dt);
typedef void (*EnemyAttackFn)(Enemy *e, float dt);
typedef void (*BulletFn)(Bullet *b, float dt);
typedef struct Animation Animation;

typedef struct BulletType
{
	float MovementSpeed;
	float Size;
	BulletFn Pattern;
} BulletType;

typedef struct EnemyType
{
	char *AnimationName;
	BulletType *BulletTypes[4];
	float MovementSpeed;
	float Size;
	EnemyMovementFn MovementPattern;
	EnemyAttackFn AttackPattern;
	int HP;
} EnemyType;

typedef struct Player
{
	Animation *animation;
	Vector2 Position;
	float HurtboxSize;

	float MovementSpeed;
	float FireRate;
	float FireTimer;

	int Lives;
	float ImmuneTime;
	bool IsAlive;
} Player;

typedef struct Bullet
{
	BulletType *Type;

	Vector2 Position;
	float Angle;
	bool FromPlayer;
	float Timer;
	bool IsAlive;
} Bullet;

typedef struct Enemy
{
	Animation *Animation;
	EnemyType *Type;

	Vector2 Position;
	Vector2 Target;
	int HP;
	bool IsAlive;
	float Timer;
} Enemy;

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
extern Enemy enemies[ENEMY_COUNT];
extern VFX vfxPool[VFX_COUNT];
extern Player player;

extern BulletType BT_PLAYER;
extern EnemyType ET_TEST;

bool HitPlayer();

Bullet *SpawnBullet(Vector2 pos, float angle, bool fromPlayer, BulletType *bulletType);
Enemy *SpawnEnemy(Vector2 pos, Vector2 to, EnemyType *enemyType);
VFX *SpawnVFX(Vector2 pos, Texture2D sprite, float dir, float lifetime);