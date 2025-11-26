#pragma once
#include "main.h"

#define SPRITE_MAX_COUNT 512
#define SFX_MAX_COUNT   32

typedef struct SpriteEntry {
    const char *key;
    Texture2D texture;
} SpriteEntry;

typedef struct SFXEntry {
    const char *ID;
    Sound sound;
} SFXEntry;

extern Font DefaultFont;
extern SpriteEntry sprites[SPRITE_MAX_COUNT];

extern const char* SPRITE_PATHS[SPRITE_MAX_COUNT];
extern const char* SFX_PATHS[SFX_MAX_COUNT];

extern SFXEntry sounds[SFX_MAX_COUNT];
extern bool loadedSounds;

void LoadAllSprites();
void DrawSprite(Texture2D sprite, Vector2 pos, float angle, Color tint);
void DrawSpriteScaled(Texture2D sprite, Vector2 pos, float angle, float scale, Color tint);
Texture2D GetSprite(const char *name);
void UnloadAllSprites();

void InitAudio();
void LoadAllSounds();
void UnloadAllSounds();