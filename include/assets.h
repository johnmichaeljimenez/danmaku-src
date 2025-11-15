#pragma once
#include "main.h"

typedef struct SpriteEntry {
    char key[512];
    Texture2D texture;
} SpriteEntry;

typedef struct SpriteManager {
    SpriteEntry *entries;
    int count;
} SpriteManager;

void LoadAllSprites();
void DrawSprite(Texture2D sprite, Vector2 pos, float angle, Color tint);
Texture2D GetSprite(const char *name);
void UnloadAllSprites();