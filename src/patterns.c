#include "patterns.h"

void EnemyMovementPattern_Straight(Enemy *e, float dt)
{
    Vector2 to = Vector2Normalize(Vector2Subtract(e->Target, e->Position));
    e->Position = Vector2Add(e->Position, Vector2Scale(to, dt * e->Type->MovementSpeed));
}

void EnemyAttackPattern_Aimed(Enemy *e, float dt)
{
    e->Timer += dt;
    if (fmodf(e->Timer, 0.8f) < dt)
    {
        Vector2 playerPos = player.Position;
        float angle = atan2f(e->Position.y - playerPos.y, playerPos.x - e->Position.x) * RAD2DEG;
        SpawnBullet(e->Position, angle, false, e->Type->BulletTypes[0]);
    }
}

void BulletPattern_Straight(Bullet *b, float dt)
{
    float rad = b->Angle * DEG2RAD;
    b->Position.x += cosf(rad) * b->Type->MovementSpeed * dt;
    b->Position.y += -sinf(rad) * b->Type->MovementSpeed * dt;
}