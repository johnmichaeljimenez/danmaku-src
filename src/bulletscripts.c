#include "bulletscripts.h"
#include "gen_bulletscripts.h"

Bullet* _spawnBullet(Bullet* from, int x, int y, int d, const char *ID1, const char *ID2, float dt)
{
    Bullet *b2 = SpawnBullet((Vector2){x, y}, d, false, ID1, ID2);
    return b2;
}

void UpdateBullet(Bullet *b, float dt)
{
    if (!b->IsAlive)
        return;

    if (b->OpIndex < b->Script->Count)
    {
        int c = b->OpIndex;
        if (c == b->RepeatTarget && b->TotalCount > 0 && b->CurrentCount < b->TotalCount && b->IntervalTimer > 0 && b->IntervalDuration > 0)
        {
            b->IntervalTimer -= dt;
            return;
        }

opstart:
        BulletScriptInstruction ins = b->Script->Instr[b->OpIndex];
        Vector2 diff;
        Bullet *b2;
        Vector2 offset, pos1, pos2, pos3;
        float minDir, maxDir, targetDir;
        float d, vel;

        float n = b->TotalCount == 0? 1.0f : (float)b->CurrentCount / (float)b->TotalCount;

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
            b2 = _spawnBullet(b, b->Position.x + ins.arg1, b->Position.y + ins.arg2, ins.arg5, ins.ID1, ins.ID2, dt);
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

        case OP_SFX:
            PlaySFXVaried(ins.ID1, 0.9f, 1.0f);
            break;

        case OP_REPEAT:
            if (ins.arg1 > 0)
            {
                b->IntervalDuration = (float)ins.arg2 / TICK_COUNT;
                b->IntervalTimer = b->IntervalDuration;
                b->RepeatTarget = c + 1;

                b->CurrentCount = 0;
                b->TotalCount = ins.arg1;

                // TraceLog(LOG_INFO, "REPEAT TO: %d BY %f x %d", c + 1, b->IntervalDuration, b->TotalCount);
            }

            break;

        case OP_PATT_RING:
            minDir = ins.arg1;
            maxDir = ins.arg2;

            targetDir = Lerp(minDir, maxDir, n);
            targetDir += 90;
            
            offset = (Vector2){
                cosf(targetDir * DEG2RAD) * ins.arg3,
                sinf(targetDir * DEG2RAD) * ins.arg3,
            };

            b2 = _spawnBullet(b, b->Position.x + offset.x, b->Position.y + offset.y, 0, ins.ID1, ins.ID2, dt);
            if (b2 == NULL)
                break;

            b2->Velocity = Vector2Scale(Vector2Normalize(offset), ins.arg4);
            break;

        case OP_PATT_RANDOM:
            minDir = ins.arg5;
            maxDir = ins.arg6;

            targetDir = GetRandomValue(minDir, maxDir);
            targetDir += 90;
            
            offset = (Vector2){
                cosf(targetDir * DEG2RAD),
                sinf(targetDir * DEG2RAD),
            };

            b2 = _spawnBullet(b, b->Position.x + offset.x + ins.arg1, b->Position.y + offset.y + ins.arg2, 0, ins.ID1, ins.ID2, dt);
            if (b2 == NULL)
                break;

            b2->Velocity = Vector2Scale(Vector2Normalize(offset), GetRandomValue(ins.arg3, ins.arg4));
            break;

        case OP_PATT_CURTAIN:
            pos1 = (Vector2){ins.arg1, ins.arg2};
            pos2 = (Vector2){ins.arg3, ins.arg4};

            pos3 = Vector2Lerp(pos1, pos2, n);
            Vector2 sweepDir = Vector2Normalize(Vector2Subtract(pos2, pos1));
            Vector2 norm = { sweepDir.y, sweepDir.x };

            b2 = _spawnBullet(b, pos3.x, pos3.y, 0, ins.ID1, ins.ID2, dt);
            if (b2 == NULL)
                break;

            b2->Velocity = Vector2Scale(norm, ins.arg5);
            break;
        }

        bool next = true;
        if (c == b->RepeatTarget)
        {
            b->CurrentCount++;

            if (b->CurrentCount < b->TotalCount)
            {
                if (b->IntervalDuration == 0)
                    goto opstart;
                else
                    b->IntervalTimer = b->IntervalDuration;

                next = false;
                return;
            }
            else
            {
                b->CurrentCount = 0;
                b->IntervalDuration = 0;
                b->IntervalTimer = 0;
                b->TotalCount = 0;
                b->RepeatTarget = -1;
            }
        }

        if (next && ins.OPCODE != OP_JUMP && ins.OPCODE != OP_WAIT)
            b->OpIndex++;
    }

    b->Position = Vector2Add(b->Position, Vector2Scale(b->Velocity, dt));
}