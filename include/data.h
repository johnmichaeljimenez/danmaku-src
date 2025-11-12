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

	bool FromPlayer;
	bool IsAlive;
} Bullet;

typedef struct Enemy
{
	Vector2 Position;
	float Direction;
	float MovementSpeed;
	float Size;
	int HP;

	bool IsAlive;
} Enemy;