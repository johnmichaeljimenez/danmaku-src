#include "bulletscripts.h"

void UpdateBullet(Bullet *b, float dt)
{
    if (!b->IsAlive)
        return;

    if (b->OpIndex < b->Script->Count)
    {
        BulletScriptInstruction ins = b->Script->Instr[b->OpIndex];
        Vector2 diff;
        float d, vel;

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
            if (b->SpawnIntervalTimer > 0)
            {
                b->SpawnIntervalTimer -= dt;
                break;
            }

            Bullet *b2 = SpawnBullet((Vector2){b->Position.x + ins.arg1, b->Position.y + ins.arg2}, ins.arg5, false, ins.ID1, ins.ID2);
            b2->Velocity.x = ins.arg3;
            b2->Velocity.y = ins.arg4;
            b->SpawnCounter++;
            b->SpawnIntervalTimer = (float)ins.arg7 / TICK_COUNT; // 60 ticks = 1 second

            if (b->SpawnCounter >= ins.arg6)
            {
                b->SpawnCounter = 0;
                b->SpawnIntervalTimer = 0;
                b->OpIndex++;
            }

            break;

        case OP_DESPAWN:
            DespawnBullet(b);
            break;

        case OP_SET_VEL:
            b->Velocity = (Vector2){ins.arg1, ins.arg2};
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
            diff = Vector2Subtract((Vector2){ins.arg3, ins.arg4}, (Vector2){ins.arg1, ins.arg2});
            d = Vector2Length(diff);

            vel = d / ((float)ins.arg5 / TICK_COUNT);
            b->Velocity = Vector2Scale(Vector2Normalize(diff), d);
            break;

        case OP_SET_GFX:
            ReuseAnimation(b->Animation, ins.ID1);
            break;

        case OP_SET_HIT:
            b->IgnoreHit = ins.arg1 == 0;
            break;
        }

        if (ins.OPCODE != OP_JUMP && ins.OPCODE != OP_WAIT && ins.OPCODE != OP_SPAWN)
            b->OpIndex++;
    }

    b->Position = Vector2Add(b->Position, Vector2Scale(b->Velocity, dt));
}

BulletScript BulletScripts[BULLET_SCRIPT_COUNT] = {
    // [0 ... BULLET_SCRIPT_COUNT-1] = {
    //     .ID = "INVALID",
    //     .Count = 0,
    // },

    // #include "gen_scripts.inc"
    {
        .ID = "bullet_player_basic",
        .Count = 1,
        .Instr = {
            [0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = -1024},
        }},

    {.ID = "bullet_enemy_basic", .Count = 1, .Instr = {[0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = 512}}},

    {.ID = "root_level_1", .Count = 10, .Instr = {
        {.OPCODE = OP_WAIT, .arg1 = 120},
        {.OPCODE = OP_SPAWN, .ID1 = "enemy_test", .arg1 = 0, .arg2 = 0, .arg3 = 0, .arg4 = 100, .arg5 = 0, .arg6 = 5, .arg7 = 30, .ID2 = NULL},
        {.OPCODE = OP_WAIT, .arg1 = 120},
        {.OPCODE = OP_SET_HIT, .arg1 = 1},
        {.OPCODE = OP_TRACK_VEL, .arg1 = 120},
        {.OPCODE = OP_WAIT, .arg1 = 120},
        {.OPCODE = OP_SET_HIT, .arg1 = 0},
        {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = 0},
        {.OPCODE = OP_SPAWN, .ID1 = "enemy_test", .arg1 = 0, .arg2 = 0, .arg3 = 0, .arg4 = 100, .arg5 = 0, .arg6 = 5, .arg7 = 30, .ID2 = NULL},
        {.OPCODE = OP_JUMP, .arg1 = 0}
    }},
    {.ID = "bullet_enemy_basic", .Count = 1, .Instr = {[0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = 512}}},

    {
        .ID = "enemy_test",
        .Count = 1,
        .Instr = {[0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = 256}},
    }};