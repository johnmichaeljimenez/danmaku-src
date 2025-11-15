#include "game.h"

bool IsCutscene;
Vector2 lastPointerPos;

static float cutsceneTimer;
static Animation *playerAnimation;

void OnCutsceneTimerDone(const char *id)
{
	IsCutscene = false;
}

void GameStart(int level)
{
	ClearAnimations();

	IsCutscene = true;
	memset(bullets, 0, sizeof(bullets));
	memset(enemies, 0, sizeof(enemies));

	playerAnimation = CreateAnimation("PlayerIdle");

	player = (Player){
		.Lives = 3,
		.ImmuneTime = 0,
		.FireRate = 0.1f,
		.FireTimer = 0,
		.HurtboxSize = 48,
		.IsAlive = true,
		.MovementSpeed = 512,
		.Position = (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT + 100}};

	lastPointerPos = player.Position;
	SetLevel(level);

	TweenManager_AddFloatFrom(&cutsceneTimer, 1, 0, 1, EASING_LINEAR, "CutsceneTimer", OnCutsceneTimerDone);
	TweenManager_AddVector2From(&player.Position, (Vector2){VIRTUAL_WIDTH * 0.5, VIRTUAL_HEIGHT + 100}, (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT - 100}, 1, EASING_EASEINOUTQUAD, "PlayerStartPosition", NULL);
}

void GameUpdate(float dt)
{
	UpdateLevel(dt);

	if (player.IsAlive)
	{
		if (player.ImmuneTime > 0)
			player.ImmuneTime -= dt;

		Vector2 inputMovement = Vector2Zero();

		if (!IsCutscene)
		{
			bool isShooting = IsKeyDown(KEY_SPACE);

			if (IsKeyDown(KEY_A))
				inputMovement.x = -1;
			else if (IsKeyDown(KEY_D))
				inputMovement.x = 1;

			if (IsKeyDown(KEY_W))
				inputMovement.y = -1;
			else if (IsKeyDown(KEY_S))
				inputMovement.y = 1;

			if (IsPointerDown())
			{
				Vector2 p = GetVirtualPointer();
				Vector2 dir = Vector2Subtract(p, lastPointerPos);
				if (Vector2Length(dir) > 0)
					inputMovement = dir;

				lastPointerPos = p;
			}

			if (isShooting)
			{
				player.FireTimer += dt;
				if (player.FireTimer >= player.FireRate)
				{
					player.FireTimer = 0;
					SpawnBullet(player.Position, 90, true, &BT_PLAYER);
				}
			}
			else
			{
				player.FireTimer = 0;
			}

			if (Vector2LengthSqr(inputMovement) > 0)
				inputMovement = Vector2Normalize(inputMovement);
		}

		player.Position.x += inputMovement.x * dt * player.MovementSpeed;
		player.Position.y += inputMovement.y * dt * player.MovementSpeed;

		player.Position = Vector2Clamp(player.Position, (Vector2){0, 0}, (Vector2){VIRTUAL_WIDTH, VIRTUAL_HEIGHT});
	}

	for (int i = 0; i < BULLET_COUNT; i++)
	{
		Bullet *b = &bullets[i];
		if (!b->IsAlive)
			continue;

		b->Type->Pattern(b, dt);

		if (b->Position.x < -100 || b->Position.x > VIRTUAL_WIDTH + 100 || b->Position.y < -100 || b->Position.y > VIRTUAL_HEIGHT + 100)
		{
			b->IsAlive = false;
		}
		else
		{
			if (b->FromPlayer && player.IsAlive)
			{
				for (int i = 0; i < ENEMY_COUNT; i++)
				{
					Enemy *e = &enemies[i];
					if (!e->IsAlive)
						continue;

					if (CheckCollisionCircles(e->Position, e->Type->Size, b->Position, b->Type->Size))
					{
						e->HP--;
						b->IsAlive = false;
						break;
					}
				}
			}
			else if (player.ImmuneTime <= 0 && player.IsAlive)
			{
				if (CheckCollisionCircles(player.Position, player.HurtboxSize, b->Position, b->Type->Size))
				{
					HitPlayer();
					b->IsAlive = false;
					break;
				}
			}
		}
	}

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		Enemy *e = &enemies[i];
		if (!e->IsAlive)
			continue;

		e->Type->MovementPattern(e, dt);

		if (e->Type->AttackPattern != NULL)
			e->Type->AttackPattern(e, dt);

		if (e->HP <= 0 || e->Position.x < -100 || e->Position.x > VIRTUAL_WIDTH + 100 || e->Position.y < -100 || e->Position.y > VIRTUAL_HEIGHT + 100)
		{
			e->IsAlive = false;
			continue;
		}
	}

	UpdateAnimations(dt);

	if (IsKeyReleased(KEY_ESCAPE))
		PauseGame();
}

void GameRender(float dt)
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		Bullet *b = &bullets[i];
		if (!b->IsAlive)
			continue;

		DrawCircleV(b->Position, b->Type->Size, b->FromPlayer ? WHITE : RED);
	}

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		Enemy *e = &enemies[i];
		if (!e->IsAlive)
			continue;

		DrawCircleV(e->Position, e->Type->Size, RED);
	}

	// if (player.TweenHitTimer > 0)
	// {
	// 	DrawCircleV(
	// 		player.Position,
	// 		player.HurtboxSize,
	// 		ColorLerp(GREEN, RED, player.TweenHitTimer));
	// }
	// else
	// {
	// 	DrawCircleV(
	// 		player.Position,
	// 		player.HurtboxSize,
	// 		player.ImmuneTime > 0 ? BLUE : player.IsAlive ? GREEN
	// 													  : RED);
	// }

	DrawSprite(playerAnimation->Clip->Frames[playerAnimation->FrameIndex], player.Position);
}

void GameQuit()
{
	ClearAnimations();
}