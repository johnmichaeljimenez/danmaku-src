#pragma once
#include "main.h"

#define SPRITE_MAX_COUNT 512
#define SFX_MAX_COUNT   32
#define SFX_SUB_MAX_COUNT   8

typedef struct SpriteEntry {
    const char *key;
    Texture2D texture;
} SpriteEntry;

typedef struct SFXEntry {
    const char *key;
    int count;
    Sound SoundAssets[SFX_SUB_MAX_COUNT];
} SFXEntry;

typedef struct SFXPath {
    int count;
    char* ID;
    char* SoundPaths[SFX_SUB_MAX_COUNT];
} SFXPath;

extern Font DefaultFont;
extern SpriteEntry sprites[SPRITE_MAX_COUNT];

extern const char* SPRITE_PATHS[SPRITE_MAX_COUNT];
extern const SFXPath SFX_PATHS[SFX_MAX_COUNT];

void LoadAllSprites();
void DrawSprite(Texture2D sprite, Vector2 pos, float angle, Color tint);
void DrawSpriteScaled(Texture2D sprite, Vector2 pos, float angle, float scale, Color tint);
Texture2D GetSprite(const char *name);
void UnloadAllSprites();