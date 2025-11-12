#pragma once

#include "main.h"
#define BULLET_COUNT 2048
#define ENEMY_COUNT 64

typedef struct Enemy Enemy;
typedef struct Bullet Bullet;
typedef void (*EnemyMovementFn)(Enemy *e, float dt);
typedef void (*EnemyAttackFn)(Enemy *e, float dt);
typedef void (*BulletFn)(Bullet *b, float dt);

typedef struct Player
{
	Vector2 Position;
	float HurtboxSize;

	float MovementSpeed;
	float FireRate;
	float FireTimer;

	bool IsAlive;
} Player;

typedef struct Bullet
{
	Vector2 Position;
	float Angle;
	float MovementSpeed;
	float Size;
	BulletFn Pattern;

	bool FromPlayer;
	float Timer;
	bool IsAlive;
} Bullet;

typedef struct Enemy
{
	Vector2 Position;
	float Direction;
	float MovementSpeed;
	float Size;
	int HP;
	EnemyMovementFn MovementPattern;
	EnemyAttackFn AttackPattern;

	bool IsAlive;
	float Timer;
} Enemy;

extern Bullet bullets[BULLET_COUNT];
extern Enemy enemies[ENEMY_COUNT];
extern Player player;

Bullet *SpawnBullet(Vector2 pos, float angle, float speed, float size, bool fromPlayer, BulletFn bulletFn);
Enemy *SpawnEnemy(Vector2 pos, float dir, float speed, float size, int hp, EnemyMovementFn movementFn, EnemyAttackFn attackFn);