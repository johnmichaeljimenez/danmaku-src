#pragma once

#include "main.h"

typedef struct Bullet Bullet;

typedef enum OpType
{
	OP_WAIT, //1 - frame (20 frames = 1 second wait)
	OP_JUMP, //1 - zero-based index to jump
	OP_SPAWN, //1 - bullet id, 
	OP_DESPAWN,
	OP_MOVE, //1 - x, 2 - y, 3 - speed
	OP_SET_VEL, //1 - xvel, 2 - yvel
	OP_ADD_VEL, //1 - xvel, 2 - yvel
	OP_TRACK_VEL, //1 - speed
	OP_SET_DIR, //1 - angle
	OP_ADD_DIR, //1 - angle
	OP_TRACK_DIR, //1 - speed
	OP_SET_GFX //gfx id
} OpType;

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
} BulletScriptInstruction;

typedef struct BulletScript
{
	const char ID[32];
	int Count;
	BulletScriptInstruction Instr[32];

} BulletScript;

#define BULLET_SCRIPT_COUNT 128
extern BulletScript BulletScripts[BULLET_SCRIPT_COUNT];
#define BULLET_SCRIPT(id_str, ...) \
    { __VA_ARGS__ },

void UpdateBullet(Bullet *b, float dt);
