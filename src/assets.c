#include "assets.h"

Font DefaultFont;
SpriteEntry sprites[SPRITE_MAX_COUNT];
SFXEntry sounds[SFX_MAX_COUNT];
bool loadedSounds;

void InitAudio()
{
	if (loadedSounds)
		return;

	InitAudioDevice();
	LoadAllSounds();
	loadedSounds = true;
}

void LoadAllSounds()
{
	for (int i = 0; i < SFX_PATH_COUNT; i++)
	{
		SFXEntry entry = sounds[i];
		entry.key = SFX_PATHS[i].ID;

		for (int j = 0; j < entry.count; j++)
		{
			entry.SoundAssets[j] = LoadSound(SFX_PATHS[i].SoundPaths[j]);
			TraceLog(LOG_INFO, "Loaded SFX: %s -> %s", SFX_PATHS[i].ID, SFX_PATHS[i].SoundPaths[j]);
		}
	}
}

void LoadAllSprites()
{
	memset(sprites, 0, sizeof(sprites));
	for (int i = 0; i < SPRITE_PATH_COUNT; i++)
	{
		sprites[i] = (SpriteEntry)
		{
			.key = SPRITE_PATHS[i],
			.texture = LoadTexture(SPRITE_PATHS[i])
		};

		TraceLog(LOG_INFO, "LOADED: %s", SPRITE_PATHS[i]);
	}
}

void DrawSprite(Texture2D sprite, Vector2 pos, float angle, Color tint)
{
	DrawSpriteScaled(sprite, pos, angle, 1, tint);
}

void DrawSpriteScaled(Texture2D sprite, Vector2 pos, float angle, float scale, Color tint)
{
    float defScale = 2.0f;
	scale *= defScale;

    Rectangle src = { 0, 0, sprite.width, sprite.height };
    Rectangle dst = {
        pos.x,
        pos.y,
        src.width  * scale,
        src.height * scale
    };

    Vector2 origin = {
        dst.width  * 0.5f,
        dst.height * 0.5f
    };

    DrawTexturePro(sprite, src, dst, origin, -angle, tint);
}

Texture2D GetSprite(const char *name)
{
	for (int i = 0; i < SPRITE_PATH_COUNT; i++)
	{
		if (strcmp(sprites[i].key, name) == 0)
		{
			TraceLog(LOG_INFO, sprites[i].key);
			return sprites[i].texture;
		}
	}

	TraceLog(LOG_ERROR, TextFormat("CANNOT FIND SPRITE: %s", name));
}

void UnloadAllSprites()
{
	for (int i = 0; i < SPRITE_PATH_COUNT; i++)
	{
		UnloadTexture(sprites[i].texture);
	}
}
