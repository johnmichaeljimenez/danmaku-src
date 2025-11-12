#include "patterns.h"

EnemyPatternFn enemyPatterns[] = {
    EnemyPattern_Idle,
    EnemyPattern_Spiral,
};

BulletPatternFn bulletPatterns[] = {
    BulletPattern_Straight,
    BulletPattern_Sine,
};

void EnemyPattern_Idle(Enemy *e, Bullet bullets[], float dt)
{
    e->Position.y += e->MovementSpeed * dt * 0.25f;
}

void EnemyPattern_Spiral(Enemy *e, Bullet bullets[], float dt)
{
    e->Timer += dt;
    if (fmodf(e->Timer, 1.0f) < dt)
    {
        for (int i = 0; i < BULLET_COUNT; i++)
        {
            if (!bullets[i].IsAlive)
            {
                bullets[i].IsAlive = true;
                bullets[i].FromPlayer = false;
                bullets[i].Position = e->Position;
                bullets[i].Angle = e->Timer * 240;
                bullets[i].MovementSpeed = 512;
                bullets[i].Size = 12;
                bullets[i].PatternID = 0;
                break;
            }
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