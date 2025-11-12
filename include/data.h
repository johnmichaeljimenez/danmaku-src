#pragma once

#include "main.h"
#define BULLET_COUNT 64
#define ENEMY_COUNT 64

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
    int PatternID;

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
    int PatternID;

	bool IsAlive;
    float Timer;
} Enemy;

extern Bullet bullets[BULLET_COUNT];
extern Enemy enemies[ENEMY_COUNT];

Bullet *SpawnBullet(Vector2 pos, float angle, float speed, float size, bool fromPlayer, int patternID);
Enemy *SpawnEnemy(Vector2 pos, float dir, float speed, float size, int hp, int patternID);