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

Bullet *SpawnBullet(Vector2 pos, float angle, bool fromPlayer, BulletType *bulletType)
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
            b->Type = bulletType;
            b->Timer = 0;
            return b;
        }
    }

    return NULL;
}

Enemy *SpawnEnemy(Vector2 pos, float dir, EnemyType *enemyType)
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *e = &enemies[i];
        if (!e->IsAlive)
        {
            e->IsAlive = true;
            e->Position = pos;
            e->Direction = dir;
            e->Type = enemyType;
            e->HP = e->Type->HP;
            e->Timer = 0;
            return e;
        }
    }
    return NULL;
}

BulletType BT_PLAYER = (BulletType){
    .MovementSpeed = 2048,
    .Pattern = BulletPattern_Straight,
    .Size = 64
};

BulletType BT_ENEMY_GENERIC = (BulletType){
    .MovementSpeed = 512,
    .Pattern = BulletPattern_Straight,
    .Size = 32
};

EnemyType ET_TEST = (EnemyType){
    .HP = 10,
    .MovementPattern = EnemyMovementPattern_StraightDown,
    .AttackPattern = EnemyAttackPattern_Aimed,
    .MovementSpeed = 256,
    .Size = 48,
    .BulletTypes = { &BT_ENEMY_GENERIC }
};