#include "patterns.h"

void EnemyMovementPattern_StraightDown(Enemy *e, float dt)
{
    float rad = e->Direction * DEG2RAD;
    e->Position.x += cosf(rad) * e->MovementSpeed * dt;
    e->Position.y += -sinf(rad) * e->MovementSpeed * dt;
}

void EnemyAttackPattern_Aimed(Enemy *e, float dt)
{
    e->Timer += dt;
    if (fmodf(e->Timer, 0.8f) < dt)
    {
        Vector2 playerPos = player.Position;
        float angle = atan2f(e->Position.y - playerPos.y, playerPos.x - e->Position.x) * RAD2DEG;
        SpawnBullet(e->Position, angle, 350, 8, false, BulletPattern_Straight);
    }
}

void BulletPattern_Straight(Bullet *b, float dt)
{
    float rad = b->Angle * DEG2RAD;
    b->Position.x += cosf(rad) * b->MovementSpeed * dt;
    b->Position.y += -sinf(rad) * b->MovementSpeed * dt;
}