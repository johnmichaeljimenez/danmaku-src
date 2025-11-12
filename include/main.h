#pragma once

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define wrappedIndex(start, offset, count, direction) (start + offset * direction + count) % count

#include <raylib.h>

#include <stdbool.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <emscripten/emscripten.h>
#include <time.h>

#include "screen.h"
#include "data.h"