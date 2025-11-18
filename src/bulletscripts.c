#include "bulletscripts.h"

void UpdateBullet(Bullet *b, float dt)
{
    if (!b->IsAlive)
        return;

    if (b->OpIndex < b->Script->Count)
    {
        BulletScriptInstruction ins = b->Script->Instr[b->OpIndex];
        switch (ins.OPCODE)
        {
        case OP_WAIT:
            b->WaitCounter += dt;
            if (b->WaitCounter >= (float)ins.arg1 / 60.0f) // 60 ticks = 1 second
            {
                b->WaitCounter = 0;
                b->OpIndex++;
            }
            break;

        case OP_JUMP:
            b->OpIndex = ins.arg1;
            break;

        case OP_SET_VEL:
            b->Velocity = (Vector2){ins.arg1, ins.arg2};
            break;

        case OP_ADD_VEL:
            b->Velocity = Vector2Add(b->Velocity, Vector2Scale((Vector2){ins.arg1, ins.arg2}, dt));
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

        case OP_DESPAWN:
            DespawnBullet(b);
            return;

        case OP_SET_DIR:
            b->Angle = ins.arg1;

        case OP_ADD_DIR:
            b->Angle += ins.arg1;

        case OP_MOVE:
            Vector2 d = Vector2Subtract((Vector2){ins.arg3, ins.arg4}, (Vector2){ins.arg1, ins.arg2});
            b->Velocity = Vector2Scale(Vector2Normalize(d), ins.arg5);
        }

        if (ins.OPCODE != OP_JUMP && ins.OPCODE != OP_WAIT)
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
    {.ID = "bullet_enemy_basic", .Count = 1, .Instr = {[0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = 512}}}};