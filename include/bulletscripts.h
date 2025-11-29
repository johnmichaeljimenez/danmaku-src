#pragma once

#include "main.h"

#define TICK_COUNT 60.0f

typedef struct Bullet Bullet;

typedef enum OpType
{
	OP_WAIT, //1 - frame (20 frames = 1 second wait)
	OP_JUMP, //1 - zero-based index to jump
	OP_SPAWN, //ID1 - bullet id, 1 - x, 2 - y, 3 - vx, 4 - vy, 5 - d, 6 - amt, 7 - time ID2 - scriptid
	OP_DESPAWN,
	OP_MOVE, //1 - x, 2 - y, 3 - duration
	OP_SET_VEL, //1 - xvel, 2 - yvel
	OP_ADD_VEL, //1 - xvel, 2 - yvel
	OP_ROT_VEL, //1 - angle
	OP_FLIP_VEL, //1 - x mul, 2 - y mul, 3 - local
	OP_TRACK_VEL, //1 - duration
	OP_SET_DIR, //1 - angle
	OP_ADD_DIR, //1 - angle
	OP_TRACK_DIR,
	OP_SET_GFX, //ID1 - gfx id,
	OP_SET_HIT, //1 - hittable,

	OP_SFX, //ID1 - sfx id
	OP_VFX, //1 - x, 2 - y, 3 - dir, 4 - time, ID1 - vfx id

	OP_REPEAT, //1 - count, 2 - interval
	OP_MK_SPAWNS, //(MARKER ONLY)
	OP_PATT_RING, //1 - min dir, 2 - max dir, 3 - dist, 4 - vel, ID1 - bullet id, ID2 - scriptid
	OP_PATT_RANDOM, //1 - x, 2 - y, 3 - min vel, 4 - max vel, 5 - min dir, 6 - max dir, ID1 - bullet id, ID2 - scriptid
	OP_PATT_CURTAIN, //1 - x1, 2 - y1, 3 - x2, 4 - y2, 5 - vel
} OpType;

typedef enum BulletScriptFlags
{
	BFLAG_NONE = 0,
} BulletScriptFlags;

typedef struct BulletScriptInstruction
{
	OpType OPCODE;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
	int arg5;
	int arg6;
	int arg7;
	int arg8;
	const char *ID1;
	const char *ID2;
	BulletScriptFlags flags; 
} BulletScriptInstruction;

typedef struct BulletScript
{
	const char ID[32];
	int Count;
	BulletScriptInstruction Instr[512];

} BulletScript;

#define BULLET_SCRIPT_COUNT 128
extern BulletScript BulletScripts[BULLET_SCRIPT_COUNT];
#define BULLET_SCRIPT(id_str, ...) \
    { __VA_ARGS__ },

void UpdateBullet(Bullet *b, float dt);
