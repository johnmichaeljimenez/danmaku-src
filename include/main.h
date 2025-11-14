#pragma once

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define arraySize(a) (sizeof(a) / sizeof *(a))
#define wrappedIndex(start, offset, count, direction) (start + offset * direction + count) % count

#include <raylib.h>

#include <stdbool.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <math.h>
#include <emscripten/emscripten.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#include "tween.h"
#include "assets.h"
#include "screen.h"
#include "data.h"
#include "patterns.h"
#include "levels.h"
#include "menu.h"
#include "game.h"

void GoToGame(int level);
void GoToMenu();

typedef enum GameState
{
	GAMESTATE_MENU,
	GAMESTATE_INGAME,
	GAMESTATE_PAUSE,
	GAMESTATE_GAME_END
} GameState;