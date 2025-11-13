#pragma once

#include "main.h"
#define BULLET_COUNT 2048
#define ENEMY_COUNT 64

typedef struct Enemy Enemy;
typedef struct Bullet Bullet;
typedef void (*EnemyMovementFn)(Enemy *e, float dt);
typedef void (*EnemyAttackFn)(Enemy *e, float dt);
typedef void (*BulletFn)(Bullet *b, float dt);

typedef struct BulletType
{
	float MovementSpeed;
	float Size;
	BulletFn Pattern;
} BulletType;

typedef struct EnemyType
{
	BulletType *BulletTypes[4];
	float MovementSpeed;
	float Size;
	EnemyMovementFn MovementPattern;
	EnemyAttackFn AttackPattern;
	int HP;
} EnemyType;

typedef struct Player
{
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
	EnemyType *Type;

	Vector2 Position;
	float Direction;
	int HP;
	bool IsAlive;
	float Timer;
} Enemy;

extern Bullet bullets[BULLET_COUNT];
extern Enemy enemies[ENEMY_COUNT];
extern Player player;

extern BulletType BT_PLAYER;
extern EnemyType ET_TEST;

bool HitPlayer();

Bullet *SpawnBullet(Vector2 pos, float angle, bool fromPlayer, BulletType *bulletType);
Enemy *SpawnEnemy(Vector2 pos, float dir, EnemyType *enemyType);