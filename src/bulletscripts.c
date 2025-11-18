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
            b->WaitCounter += 1;
            if (b->WaitCounter >= ins.arg1)
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
        case OP_DESPAWN:
            DespawnBullet(b);
            return;
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
        .ID = "player_basic",
        .Count = 1,
        .Instr = {
            [0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = -1024, .arg3 = 0, .arg4 = 0},
            // [1] = {.OPCODE = OP_WAIT, .arg1 = 15, .arg2 = 0, .arg3 = 0, .arg4 = 0},
            // [2] = {.OPCODE = OP_DESPAWN, .arg1 = 0, .arg2 = 0, .arg3 = 0, .arg4 = 0},
        }},
    {.ID = "enemy_basic", .Count = 1, .Instr = {[0] = {.OPCODE = OP_SET_VEL, .arg1 = 0, .arg2 = 512, .arg3 = 0, .arg4 = 0}}}};