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
	memset(sounds, 0, sizeof(sounds));

	for (int i = 0; i < SFX_PATH_COUNT; i++)
    {
        Sound base = LoadSound(SFX_PATHS[i]);

        sounds[i] = (SFXEntry){
            .ID = SFX_PATHS[i],
            .baseSound = base,
            .nextAvailable = 0
        };

        for (int j = 0; j < SFX_POOL_SIZE; j++)
        {
            sounds[i].pool[j] = LoadSoundAlias(base);
        }

        TraceLog(LOG_INFO, "Loaded SFX with pool: %s", SFX_PATHS[i]);
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
		if (TextIsEqual(sprites[i].key, name))
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

void UnloadAllSounds()
{
	for (int i = 0; i < SFX_PATH_COUNT; i++)
    {
        for (int j = 0; j < SFX_POOL_SIZE; j++)
        {
            if (IsSoundValid(sounds[i].pool[j]))
                UnloadSoundAlias(sounds[i].pool[j]);
        }
		
        if (IsSoundValid(sounds[i].baseSound))
            UnloadSound(sounds[i].baseSound);
    }
	
    loadedSounds = false;
}

void PlaySFX(const char *id)
{
    if (!loadedSounds)
        return;
        
    for (int i = 0; i < SFX_PATH_COUNT; i++)
    {
        if (TextIsEqual(sounds[i].ID, id))
        {
            int idx = sounds[i].nextAvailable;
            Sound alias = sounds[i].pool[idx];

            if (IsSoundPlaying(alias))
                StopSound(alias);

            PlaySound(alias);

            sounds[i].nextAvailable = (idx + 1) % SFX_POOL_SIZE;
            return;
        }
    }

    TraceLog(LOG_WARNING, "SFX not found: %s", id);
}

void PlaySFXVaried(const char *id, float volume, float pitch)
{
    if (!loadedSounds)
        return;

    for (int i = 0; i < SFX_PATH_COUNT; i++)
    {
        if (TextIsEqual(sounds[i].ID, id))
        {
            int idx = sounds[i].nextAvailable;
            Sound alias = sounds[i].pool[idx];

            SetSoundVolume(alias, volume);
            SetSoundPitch(alias, pitch + GetRandomValue(-10, 10) * 0.01f);

            if (IsSoundPlaying(alias)) StopSound(alias);
            PlaySound(alias);

            sounds[i].nextAvailable = (idx + 1) % SFX_POOL_SIZE;
            return;
        }
    }
}