#pragma once

#include "main.h"

typedef struct Bullet Bullet;
typedef struct Enemy Enemy;

typedef void (*EnemyPatternFn)(Enemy *e, float dt);
typedef void (*BulletPatternFn)(Bullet *b, float dt);

extern EnemyPatternFn enemyMovementPatterns[];
extern EnemyPatternFn enemyAttackPatterns[];
extern BulletPatternFn bulletPatterns[];

void EnemyMovementPattern_Idle(Enemy *e, float dt);
void EnemyAttackPattern_Spiral(Enemy *e, float dt);

void BulletPattern_Straight(Bullet *b, float dt);
void BulletPattern_Sine(Bullet *b, float dt);