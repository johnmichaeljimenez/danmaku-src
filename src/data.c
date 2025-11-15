#include "data.h"

Bullet bullets[BULLET_COUNT] = {0};
Enemy enemies[ENEMY_COUNT] = {0};
VFX vfxPool[VFX_COUNT] = {0};

Player player;

bool HitPlayer()
{
    player.Lives -= 1;

    VFX *vfx = SpawnVFX(player.Position, player.animation->Clip->Frames[player.animation->FrameIndex], 0, 1.0);
    vfx->Additive = true;
    vfx->Tint = (Color){255, 0, 0, 255};

    if (player.Lives <= 0)
    {
        player.IsAlive = false;

        TweenManager_AddFloatFrom(&vfx->Scale, 1, 10, 0.8f, EASING_EASEOUTQUAD, "VFX-PlayerDeath1", NULL);
        TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-PlayerDeath2", NULL);
    }
    else
    {
        player.ImmuneTime = 1.0f;

        TweenManager_AddFloatFrom(&vfx->Scale, 1, 2, 0.8f, EASING_EASEOUTQUAD, "VFX-PlayerDeath1", NULL);
        TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-PlayerDeath2", NULL);
    }

    return player.IsAlive;
}

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

Enemy *SpawnEnemy(Vector2 pos, Vector2 to, EnemyType *enemyType)
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *e = &enemies[i];
        if (!e->IsAlive)
        {
            e->IsAlive = true;
            e->Position = pos;
            e->Target = to;
            e->Type = enemyType;
            e->HP = e->Type->HP;
            e->MovementTimer = 0;
            e->AttackTimer = 0;
            e->Animation = CreateAnimation(e->Type->AnimationName);
            e->MovementPattern = e->Type->MovementPattern;
            e->AttackPattern = e->Type->AttackPattern;

            return e;
        }
    }

    return NULL;
}

VFX *SpawnVFX(Vector2 pos, Texture2D sprite, float dir, float lifetime)
{
    for (int i = 0; i < VFX_COUNT; i++)
    {
        VFX *v = &vfxPool[i];
        if (!v->IsAlive)
        {
            v->ID = i;
            v->IsAlive = true;
            v->Position = pos;
            v->Sprite = sprite;
            v->Direction = dir;

            v->Lifetime = lifetime;
            v->Timer = 0;

            v->Alpha = 1;
            v->Scale = 1;
            v->Tint = WHITE;
            v->Additive = false;

            return v;
        }
    }

    return NULL;
}

BulletType BT_PLAYER = (BulletType){
    .MovementSpeed = 1024,
    .Pattern = BulletPattern_Straight,
    .Size = 16};

BulletType BT_ENEMY_GENERIC = (BulletType){
    .MovementSpeed = 400,
    .Pattern = BulletPattern_Straight,
    .Size = 8};

EnemyType ET_TEST = (EnemyType){
    .AnimationName = "EnemyGeneric",
    .HP = 10,
    .MovementPattern = EnemyMovementPattern_Straight,
    .AttackPattern = NULL,
    .MovementSpeed = 256,
    .Size = 16,
    .BulletTypes = {&BT_ENEMY_GENERIC}};

EnemyType ET_BOSS1 = (EnemyType){
    .AnimationName = "EnemyGeneric",
    .HP = 200,
    .IsBoss = true,
    .MovementPattern = EnemyMovementPattern_Boss1,
    .AttackPattern = EnemyAttackPattern_Boss1,
    .MovementSpeed = 256,
    .Size = 32,
    .BulletTypes = {&BT_ENEMY_GENERIC}
};