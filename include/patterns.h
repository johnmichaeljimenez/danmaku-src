#pragma once

#include "main.h"

typedef struct Bullet Bullet;
typedef struct Enemy Enemy;

void EnemyMovementPattern_Straight(Enemy *e, float dt);
void EnemyAttackPattern_Aimed(Enemy *e, float dt);
void BulletPattern_Straight(Bullet *b, float dt);