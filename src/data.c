#include "data.h"

Bullet bullets[BULLET_COUNT] = {0};
Enemy enemies[ENEMY_COUNT] = {0};

Bullet *SpawnBullet(Vector2 pos, float angle, float speed, float size, bool fromPlayer, int patternID)
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		Bullet *b = &bullets[i];
		if (!b->IsAlive)
		{
			b->IsAlive = true;
			b->FromPlayer = fromPlayer;
			b->Position = pos;
			b->Angle = angle;
			b->MovementSpeed = speed;
			b->Size = size;
			b->PatternID = patternID;
			b->Timer = 0;
			return b;
		}
	}

	return NULL;
}

Enemy *SpawnEnemy(Vector2 pos, float dir, float speed, float size, int hp, int patternID)
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *e = &enemies[i];
        if (!e->IsAlive)
        {
            e->IsAlive = true;
            e->Position = pos;
            e->Direction = dir;
            e->MovementSpeed = speed;
            e->Size = size;
            e->HP = hp;
            e->PatternID = patternID;
            e->Timer = 0;
            return e;
        }
    }
    return NULL;
}
