#include "data.h"

Bullet bullets[BULLET_COUNT] = {0};
Enemy enemies[ENEMY_COUNT] = {0};
Player player = (Player){
    .FireRate = 0.1f,
    .FireTimer = 0,
    .HurtboxSize = 48,
    .IsAlive = true,
    .MovementSpeed = 512,
    .Position = (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT - 100}
};

Bullet *SpawnBullet(Vector2 pos, float angle, float speed, float size, bool fromPlayer, BulletFn bulletFn)
{
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *b = &bullets[i];
        if (!b->IsAlive)
        {
            b->IsAlive = true;
            b->FromPlayer = fromPlayer;
            b->Position = pos;
            b->Angle = angle;
            b->MovementSpeed = speed;
            b->Size = size;
            b->Pattern = bulletFn;
            b->Timer = 0;
            return b;
        }
    }

    return NULL;
}

Enemy *SpawnEnemy(Vector2 pos, float dir, float speed, float size, int hp, EnemyMovementFn movementFn, EnemyAttackFn attackFn)
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *e = &enemies[i];
        if (!e->IsAlive)
        {
            e->IsAlive = true;
            e->Position = pos;
            e->Direction = dir;
            e->MovementSpeed = speed;
            e->Size = size;
            e->HP = hp;
            e->MovementPattern = movementFn;
            e->AttackPattern = attackFn;
            e->Timer = 0;
            return e;
        }
    }
    return NULL;
}
