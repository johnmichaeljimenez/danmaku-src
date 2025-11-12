#pragma once

#include "main.h"

typedef void (*EnemyPatternFn)(Enemy *e, Bullet bullets[], float dt);
typedef void (*BulletPatternFn)(Bullet *b, float dt);

extern EnemyPatternFn enemyPatterns[];
extern BulletPatternFn bulletPatterns[];

void EnemyPattern_Idle(Enemy *e, Bullet bullets[], float dt);
void EnemyPattern_Spiral(Enemy *e, Bullet bullets[], float dt);

void BulletPattern_Straight(Bullet *b, float dt);
void BulletPattern_Sine(Bullet *b, float dt);