#include "patterns.h"

void EnemyMovementPattern_Straight(Enemy *e, float dt)
{
    Vector2 to = Vector2Normalize(Vector2Subtract(e->Target, e->Position));
    e->Position = Vector2Add(e->Position, Vector2Scale(to, dt * e->Type->MovementSpeed));
}

void EnemyMovementPattern_Boss1(Enemy *e, float dt)
{
    Vector2 to = Vector2Subtract(e->Target, e->Position);

    if (Vector2Length(to) <= 8)
    {
        e->Target = Vector2Lerp(e->Position, player.Position, 0.2);
    }

    if (e->MovementTimer < 10.0f)
        e->Position = Vector2Add(e->Position, Vector2Scale(Vector2Normalize(to), dt * e->Type->MovementSpeed));

    if (e->MovementTimer > 11.0f)
        e->MovementTimer = 0.0f;
}

void EnemyAttackPattern_Aimed(Enemy *e, float dt)
{
    if (fmodf(e->AttackTimer, 0.8f) < dt)
    {
        Vector2 playerPos = player.Position;
        float angle = atan2f(e->Position.y - playerPos.y, playerPos.x - e->Position.x) * RAD2DEG;
        SpawnBullet(e->Position, angle, false, e->Type->BulletTypes[0]);
    }
}

void EnemyAttackPattern_Boss1(Enemy *e, float dt)
{
    if (e->MovementTimer >= 11.0f)
    {
        for (int i = 0; i < 360; i += 10)
        {
            SpawnBullet(e->Position, i, false, e->Type->BulletTypes[0]);
        }
    }
    else if (fmodf(e->AttackTimer, 3.0f) < dt)
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