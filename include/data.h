#pragma once

#include "main.h"

typedef struct Player
{
	Vector2 Position;
	Vector2 HurtboxSize;
	
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