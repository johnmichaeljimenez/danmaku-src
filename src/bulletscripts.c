#include "bulletscripts.h"
#include "gen_bulletscripts.h"

Bullet* _spawnBullet(Bullet* from, int x, int y, int d, const char *ID1, const char *ID2, int amt, int t, float dt)
{
    if (from->SpawnIntervalTimer > 0)
    {
        from->SpawnIntervalTimer -= dt;
        return NULL;
    }

    Bullet *b2 = SpawnBullet((Vector2){x, y}, d, false, ID1, ID2);
    from->SpawnCounter++;
    from->SpawnIntervalTimer = (float)t / TICK_COUNT; // 60 ticks = 1 second

    if (from->SpawnCounter >= amt)
    {
        from->SpawnCounter = 0;
        from->SpawnIntervalTimer = 0;
        from->OpIndex++;
    }

    return b2;
}

void UpdateBullet(Bullet *b, float dt)
{
    if (!b->IsAlive)
        return;

    if (b->OpIndex < b->Script->Count)
    {
        BulletScriptInstruction ins = b->Script->Instr[b->OpIndex];
        Vector2 diff;
        Bullet *b2;
        float n;
        float d, vel;
        int c = b->OpIndex;

        switch (ins.OPCODE)
        {
        case OP_WAIT:
            b->WaitCounter += dt;
            if (b->WaitCounter >= (float)ins.arg1 / TICK_COUNT) // 60 ticks = 1 second
            {
                b->WaitCounter = 0;
                b->OpIndex++;
            }
            break;

        case OP_JUMP:
            b->OpIndex = ins.arg1;
            break;

        case OP_SPAWN: 
            b2 = _spawnBullet(b, b->Position.x + ins.arg1, b->Position.y + ins.arg2, ins.arg5, ins.ID1, ins.ID2, ins.arg6, ins.arg7, dt);
            if (b2 == NULL)
                break;

            b2->Velocity.x = ins.arg3;
            b2->Velocity.y = ins.arg4;
            break;

        case OP_DESPAWN:
            DespawnBullet(b);
            break;

        case OP_SET_VEL:
            b->Velocity = (Vector2){ins.arg1, ins.arg2};
            // TraceLog(LOG_INFO, "%d %d", ins.arg1, ins.arg2);
            break;

        case OP_ADD_VEL:
            b->Velocity = Vector2Add(b->Velocity, Vector2Scale((Vector2){ins.arg1, ins.arg2}, dt));
            break;

        case OP_TRACK_VEL:
            diff = Vector2Subtract(player.Position, b->Position);
            d = Vector2Length(diff);

            vel = d / ((float)ins.arg1 / TICK_COUNT);

            b->Velocity = Vector2Scale(Vector2Normalize(diff), vel);
            break;

        case OP_FLIP_VEL:
            int vx = b->Velocity.x;
            int vy = b->Velocity.y;

            if (ins.arg3 == 1)
            {
                vx *= ins.arg1;
                vy *= ins.arg2;
            }
            else
            {
                vx = abs(vx);
                vy = abs(vy);

                if (ins.arg1 > 0)
                    vx = b->Position.x >= VIRTUAL_WIDTH * 0.5 ? -vx : vx;

                if (ins.arg2 > 0)
                    vy = b->Position.y >= VIRTUAL_HEIGHT * 0.5 ? -vy : vy;
            }

            b->Velocity = (Vector2){vx, vy};
            break;

        case OP_ROT_VEL:
            b->Velocity = Vector2Rotate(b->Velocity, ins.arg1 * DEG2RAD);
            break;

        case OP_SET_DIR:
            b->Angle = ins.arg1;
            break;

        case OP_ADD_DIR:
            b->Angle += ins.arg1;
            break;

        case OP_TRACK_DIR:
            b->Angle = atan2f(b->Position.y - player.Position.y, player.Position.x - b->Position.x) * RAD2DEG;
            break;

        case OP_MOVE:
            diff = Vector2Subtract((Vector2){ins.arg1, ins.arg2}, b->Position);
            d = Vector2Length(diff);

            vel = d / ((float)ins.arg3 / TICK_COUNT);
            b->Velocity = Vector2Scale(Vector2Normalize(diff), d);
            break;

        case OP_SET_GFX:
            ReuseAnimation(b->Animation, ins.ID1);
            break;

        case OP_SET_HIT:
            b->IgnoreHit = ins.arg1 == 0;
            break;

        case OP_PATT_RING:
        patt_spawn:
            n = (float)b->SpawnCounter/(float)ins.arg5;
            float a = (Lerp(ins.arg4 >= 0? 0.0f : 360.0f, ins.arg4 >= 0? 360.0f : 0.0f, n)+90);
            
            b2 = _spawnBullet(b, b->Position.x + ins.arg1, b->Position.y + ins.arg2, 0, ins.ID1, ins.ID2, ins.arg5, ins.arg6, dt);
            if (b2 == NULL)
                break;

            b2->Velocity.x = cosf(a * DEG2RAD) * ins.arg3;
            b2->Velocity.y = sinf(a * DEG2RAD) * ins.arg3;
            if (ins.arg6 < 0 && c == b->OpIndex)
                goto patt_spawn;

            break;
        }

        if (ins.OPCODE != OP_JUMP && ins.OPCODE != OP_WAIT && ins.OPCODE != OP_SPAWN && ins.OPCODE < OP_MK_SPAWNS)
            b->OpIndex++;
    }

    b->Position = Vector2Add(b->Position, Vector2Scale(b->Velocity, dt));
}