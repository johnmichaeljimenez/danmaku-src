#include "data.h"

Bullet bullets[BULLET_COUNT] = {0};
VFX vfxPool[VFX_COUNT] = {0};

Player player;

bool HitPlayer()
{
    if (!player.IsAlive || player.ImmuneTime > 0 || IsCutscene || IsDialogueActive)
    {
        return false;
    }

    player.Lives -= 1;

    VFX *vfx = SpawnVFX(player.Position, player.Animation->Clip->Frames[player.Animation->FrameIndex], 0, 1.0);
    vfx->Additive = true;
    vfx->Tint = (Color){255, 0, 0, 255};

    if (player.Lives <= 0)
    {
        player.IsAlive = false;

        TweenManager_AddFloatFrom(&vfx->Scale, 1, 10, 0.8f, EASING_EASEOUTQUAD, "VFX-PlayerDeath1", OnEndLevel);
        TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-PlayerDeath2", NULL);

        vfx = SpawnVFX(player.Position, GetSprite(SPRITE_VFX_RING_BLUR), 0, 1.0f);
        vfx->Additive = true;
        vfx->Tint = (Color){255, 0, 0, 255};

        TweenManager_AddFloatFrom(&vfx->Scale, 1, 5, 0.8f, EASING_EASEOUTQUAD, "VFX-PlayerDeath3", NULL);
        TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-PlayerDeath4", NULL);
    }
    else
    {
        player.ImmuneTime = 1.1f;

        TweenManager_AddFloatFrom(&vfx->Scale, 1, 2, 0.8f, EASING_EASEOUTQUAD, "VFX-PlayerDeath1", NULL);
        TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-PlayerDeath2", NULL);
    }

    return player.IsAlive;
}

void DespawnBullet(Bullet *b)
{
    if (!b->IsAlive)
        return;

    b->IsAlive = false;
    RemoveAnimation(b->Animation);
}

Bullet *SpawnBullet(Vector2 pos, float angle, bool fromPlayer, const char *bulletType, const char *scriptOverride)
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

            for (int j = 0; j < BULLET_TYPE_COUNT; j++)
            {
                BulletType *b2 = &bulletTypes[j];
                if (TextIsEqual(b2->ID, bulletType))
                {
                    b->Type = b2;
                    break;
                }
            }

            b->HP = b->Type->HP;
            b->OpIndex = 0;
            b->WaitCounter = 0;
            
            b->CurrentCount = 0;
            b->IntervalDuration = 0;
            b->IntervalTimer = 0;
            b->TotalCount = 0;
            b->RepeatTarget = -1;
            
            b->Animation = CreateAnimation(b->Type->AnimationName);

            const char *scriptID = scriptOverride == NULL ? b->Type->ScriptName : scriptOverride;
            for (int j = 0; j < BULLET_SCRIPT_COUNT; j++)
            {
                BulletScript *b2 = &BulletScripts[j];
                if (TextIsEqual(b2->ID, scriptID))
                {
                    b->Script = b2;
                    break;
                }
            }

            b->Timer = 0;

            if (b->Type->SFXName != NULL)
                PlaySFXVaried(b->Type->SFXName, 0.5f, 1.0f);

            return b;
        }
    }

        TraceLog(LOG_ERROR, "OUT OF BULLETS");
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

        TraceLog(LOG_ERROR, "OUT OF VFX");
    return NULL;
}

Bullet *RootBullet;
BulletType bulletTypes[BULLET_TYPE_COUNT] =
    {
        {
            .ID = "enemy_boss",
            .ScriptName = "",
            .AnimationName = "PlayerIdle",
            .Size = 16,
            .HP = 500,
            .IsBoss = true,
            .FixedRotation = true
        },
        {
            .ID = "bullet_player_basic",
            .ScriptName = "bullet_player_basic",
            .AnimationName = "PlayerBulletDefault",
            .Size = 16
        },
        {
            .ID = "enemy_test",
            .ScriptName = "enemy_test",
            .HP = 0,
            .AnimationName = "EnemyGeneric",
            .Size = 16,
            .SFXName = SFX_GENERIC_FIRE
        },
        {   .ID = "bullet_enemy_basic",
            .ScriptName = "bullet_enemy_basic",
            .AnimationName = "EnemyBulletDefault",
            .Size = 14,
            .SFXName = SFX_GENERIC_FIRE
        }
    };