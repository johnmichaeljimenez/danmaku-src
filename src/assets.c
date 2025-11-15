#include "assets.h"

static SpriteManager spriteManager = {0};

static int EndsWithPNG(const char *filename)
{
	const char *ext = strrchr(filename, '.');
	return ext && (strcasecmp(ext, ".png") == 0);
}

static void ScanFolderRecursive(const char *basePath, const char *relativePath)
{
	char fullPath[1024];
	snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, relativePath);

	DIR *dir = opendir(fullPath);
	if (!dir)
		return;

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		char newRelativePath[1024];
		if (relativePath[0] != '\0')
			snprintf(newRelativePath, sizeof(newRelativePath), "%s/%s", relativePath, entry->d_name);
		else
			snprintf(newRelativePath, sizeof(newRelativePath), "%s", entry->d_name);

		char newFullPath[1024];
		snprintf(newFullPath, sizeof(newFullPath), "%s/%s", basePath, newRelativePath);

		struct stat st;
		stat(newFullPath, &st);

		if (S_ISDIR(st.st_mode))
		{
			ScanFolderRecursive(basePath, newRelativePath);
		}
		else if (EndsWithPNG(entry->d_name))
		{
			spriteManager.count++;
			spriteManager.entries = realloc(spriteManager.entries,
											spriteManager.count * sizeof(SpriteEntry));

			SpriteEntry *e = &spriteManager.entries[spriteManager.count - 1];

			snprintf(e->key, sizeof(e->key), "%s", newRelativePath);
			char *dot = strrchr(e->key, '.');
			if (dot)
				*dot = '\0';

			e->texture = LoadTexture(newFullPath);

			TraceLog(LOG_INFO, "Loaded sprite: %s (%s)\n", e->key, newFullPath);
		}
	}

	closedir(dir);
}

void LoadAllSprites()
{
	ScanFolderRecursive("assets", "");
}

void DrawSprite(Texture2D sprite, Vector2 pos)
{
	pos.x -= sprite.width * 0.5;
	pos.y -= sprite.height * 0.5;
	DrawTexture(sprite, pos.x, pos.y, WHITE);
}

Texture2D GetSprite(const char *name)
{
	for (int i = 0; i < spriteManager.count; i++)
	{
		if (strcmp(spriteManager.entries[i].key, name) == 0)
			return spriteManager.entries[i].texture;
	}

	TraceLog(LOG_ERROR, TextFormat("CANNOT FIND SPRITE: %s", name));
}

void UnloadAllSprites()
{
	for (int i = 0; i < spriteManager.count; i++)
	{
		UnloadTexture(spriteManager.entries[i].texture);
	}
	free(spriteManager.entries);
	spriteManager.entries = NULL;
	spriteManager.count = 0;
}