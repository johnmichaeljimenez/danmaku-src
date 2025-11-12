#include "patterns.h"

EnemyPatternFn enemyMovementPatterns[] = {
    EnemyMovementPattern_Idle,
};

EnemyPatternFn enemyAttackPatterns[] = {
    EnemyAttackPattern_Spiral,
};

BulletPatternFn bulletPatterns[] = {
    BulletPattern_Straight,
    BulletPattern_Sine,
};

void EnemyMovementPattern_Idle(Enemy *e, float dt)
{
    e->Position.y += e->MovementSpeed * dt * 0.25f;
}

void EnemyAttackPattern_Spiral(Enemy *e, float dt)
{
    e->Timer += dt;
    if (fmodf(e->Timer, 0.1f) < dt)
    {
        float baseAngle = e->Timer * 180;
        for (int i = 0; i < 10; i++)
        {
            float angle = baseAngle + (360.0f / 10) * i;
            SpawnBullet(e->Position, angle, 400, 12, false, 0);
        }
    }
}

void BulletPattern_Straight(Bullet *b, float dt)
{
    float rad = b->Angle * DEG2RAD;
    b->Position.x += cosf(rad) * b->MovementSpeed * dt;
    b->Position.y += -sinf(rad) * b->MovementSpeed * dt;
}

void BulletPattern_Sine(Bullet *b, float dt)
{
    b->Timer += dt;
    float rad = b->Angle * DEG2RAD;
    b->Position.x += cosf(rad) * b->MovementSpeed * dt + sinf(b->Timer * 20) * 50 * dt;
    b->Position.y += -sinf(rad) * b->MovementSpeed * dt;
}