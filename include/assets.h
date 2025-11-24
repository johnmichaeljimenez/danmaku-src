#pragma once
#include "main.h"

#define SPRITE_MAX_COUNT 512

typedef struct SpriteEntry {
    const char *key;
    Texture2D texture;
} SpriteEntry;

extern Font DefaultFont;
extern SpriteEntry sprites[SPRITE_MAX_COUNT];
extern const char* SPRITE_PATHS[SPRITE_MAX_COUNT];

void LoadAllSprites();
void DrawSprite(Texture2D sprite, Vector2 pos, float angle, Color tint);
void DrawSpriteScaled(Texture2D sprite, Vector2 pos, float angle, float scale, Color tint);
Texture2D GetSprite(const char *name);
void UnloadAllSprites();