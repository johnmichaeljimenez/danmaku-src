#include "game.h"

bool IsCutscene;

Vector2 lastPointerPos;
bool lastPointerSet;

static float cutsceneTimer;
static int playerMovementState;

Texture2D playerDefaultBullet;

void OnCutsceneTimerDone(const char *id)
{
	IsCutscene = false;
}

void OnEndLevel(const char *id)
{
	EndGame(player.IsAlive);
}

static void EndLevel()
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (!bullets[i].IsAlive)
			continue;

		if (!bullets[i].FromPlayer)
			bullets[i].IsAlive = false;
	}

	IsCutscene = true;
	TweenManager_AddFloatFrom(&cutsceneTimer, 1, 0, 5, EASING_LINEAR, "CutsceneTimer", OnCutsceneTimerDone);
	TweenManager_AddVector2(&player.Position, (Vector2){player.Position.x, -100}, 5, EASING_EASEINOUTQUAD, "PlayerTweenPosition", OnEndLevel);

	ReuseAnimation(player.Animation, "PlayerIdle");
	lastPointerSet = false;
}

void ClearGameplayData()
{
	memset(bullets, 0, sizeof(bullets));
	memset(enemies, 0, sizeof(enemies));
	memset(vfxPool, 0, sizeof(vfxPool));
	memset(Animations, 0, sizeof(Animations));
}

void GameStart(int level)
{
	playerDefaultBullet = GetSprite("bullet/player-default");
	ClearGameplayData();

	IsCutscene = true;

	player = (Player){
		.Lives = 3,
		.ImmuneTime = 0,
		.FireRate = 0.1f,
		.FireTimer = 0,
		.HurtboxSize = 8,
		.IsAlive = true,
		.MovementSpeed = 400,
		.Position = (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT + 100}};

	player.Animation = CreateAnimation("PlayerIdle");

	lastPointerPos = player.Position;
	SetLevel(level);

	TweenManager_AddFloatFrom(&cutsceneTimer, 1, 0, 1, EASING_LINEAR, "CutsceneTimer", OnCutsceneTimerDone);
	TweenManager_AddVector2From(&player.Position, (Vector2){VIRTUAL_WIDTH * 0.5, VIRTUAL_HEIGHT + 100}, (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT - 100}, 1, EASING_EASEINOUTQUAD, "PlayerTweenPosition", NULL);
}

void GameUpdate(float dt)
{
	if (IsKeyPressed(KEY_M))
	{
		EndLevel();
		return;
	}

	if (IsKeyPressed(KEY_N))
	{
		HitPlayer();
		return;
	}

	bool endedLevel = false;

	UpdateLevel(dt);

	if (player.IsAlive)
	{
		if (player.ImmuneTime > 0)
			player.ImmuneTime -= dt;

		Vector2 inputMovement = Vector2Zero();

		if (!IsCutscene)
		{
			bool isShooting = player.ImmuneTime <= 0; // IsKeyDown(KEY_SPACE);

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
				if (!lastPointerSet)
				{
					lastPointerPos = GetVirtualPointer();
					lastPointerSet = true;
				}

				Vector2 p = GetVirtualPointer();
				Vector2 dir = Vector2Subtract(p, lastPointerPos);
				if (Vector2Length(dir) > 0)
					inputMovement = Vector2Normalize(dir);

				// lastPointerPos = p;
			}
			else
			{
				lastPointerSet = false;
			}

			if (isShooting)
			{
				player.FireTimer += dt;
				if (player.FireTimer >= player.FireRate)
				{
					player.FireTimer = 0;
					SpawnBullet((Vector2){player.Position.x - 24, player.Position.y}, 90, true, &BT_PLAYER);
					SpawnBullet((Vector2){player.Position.x + 24, player.Position.y}, 90, true, &BT_PLAYER);
				}
			}
			else
			{
				player.FireTimer = 0;
			}

			if (Vector2LengthSqr(inputMovement) > 0)
				inputMovement = Vector2Normalize(inputMovement);

			if (inputMovement.x > 0)
			{
				if (playerMovementState != 1)
				{
					playerMovementState = 1;
					ReuseAnimation(player.Animation, "PlayerRight");
				}
			}
			else if (inputMovement.x < 0)
			{
				if (playerMovementState != -1)
				{
					playerMovementState = -1;
					ReuseAnimation(player.Animation, "PlayerLeft");
				}
			}
			else
			{
				if (playerMovementState != 0)
				{
					playerMovementState = 0;
					ReuseAnimation(player.Animation, "PlayerIdle");
				}
			}
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

						VFX *vfx = SpawnVFX(e->Position, e->Animation->Clip->Frames[e->Animation->FrameIndex], 0, 0.2f);
						vfx->Additive = true;

						TweenManager_AddFloatFrom(&vfx->Alpha, 0.2, 0, vfx->Lifetime, EASING_EASEINQUAD, TextFormat("VFX-Enemy-%d-1", vfx->ID), NULL);
						TweenManager_AddFloatFrom(&vfx->Scale, 1, 2, vfx->Lifetime, EASING_EASEINQUAD, TextFormat("VFX-Enemy-%d-2", vfx->ID), NULL);

						break;
					}
				}
			}
			else if (player.ImmuneTime <= 0 && player.IsAlive && !IsCutscene)
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

		if (e->HP <= 0 || e->Position.x < -100 || e->Position.x > VIRTUAL_WIDTH + 100 || e->Position.y < -100 || e->Position.y > VIRTUAL_HEIGHT + 100)
		{
			e->IsAlive = false;
			RemoveAnimation(e->Animation);

			if (e->Type->IsBoss)
			{
				VFX *vfx = SpawnVFX(e->Position, e->Animation->Clip->Frames[e->Animation->FrameIndex], 0, 1.0);
				vfx->Additive = true;

				TweenManager_AddFloatFrom(&vfx->Scale, 1, 10, 0.8f, EASING_EASEOUTQUAD, "VFX-BossDeath1", NULL);
				TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-BossDeath2", NULL);

				endedLevel = true;
			}

			continue;
		}

		e->MovementPattern(e, dt);
		e->MovementTimer += dt;

		if (e->AttackPattern != NULL)
			e->AttackPattern(e, dt);

		e->AttackTimer += dt;

		if (CheckCollisionCircles(e->Position, e->Type->Size, player.Position, player.HurtboxSize))
		{
			HitPlayer();
		}
	}

	if (endedLevel)
	{
		EndLevel();
	}

	UpdateAnimations(dt);

	for (int i = 0; i < VFX_COUNT; i++)
	{
		VFX *vfx = &vfxPool[i];
		if (!vfx->IsAlive)
			continue;

		vfx->Timer += dt;
		if (vfx->Timer >= vfx->Lifetime)
			vfx->IsAlive = false;
	}

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

		if (b->FromPlayer)
		{
			DrawSprite(playerDefaultBullet, b->Position, b->Angle, WHITE);
			continue;
		}

		DrawCircleV(b->Position, b->Type->Size, b->FromPlayer ? WHITE : RED);
	}

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		Enemy *e = &enemies[i];
		if (!e->IsAlive)
			continue;

		DrawSprite(e->Animation->Clip->Frames[e->Animation->FrameIndex], e->Position, 0, WHITE);
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

	if (player.IsAlive)
	{
		DrawSprite(player.Animation->Clip->Frames[player.Animation->FrameIndex], player.Position, 0, player.ImmuneTime > 0 ? (Color){255, 0, 0, 255} : WHITE);
		DrawCircleV(player.Position, player.HurtboxSize, player.ImmuneTime > 0 ? (Color){255, 0, 0, 255} : WHITE);
	}

	for (int i = 0; i < VFX_COUNT; i++)
	{
		VFX *vfx = &vfxPool[i];
		if (!vfx->IsAlive)
			continue;

		float scale = 2.0f * vfx->Scale;

		Rectangle src = {0, 0, vfx->Sprite.width, vfx->Sprite.height};
		Rectangle dst = {
			vfx->Position.x,
			vfx->Position.y,
			src.width * scale,
			src.height * scale};

		Vector2 origin = {
			dst.width * 0.5f,
			dst.height * 0.5f};

		if (vfx->Additive)
		{
			BeginBlendMode(BLEND_ADDITIVE);
		}

		DrawTexturePro(vfx->Sprite, src, dst, origin, -vfx->Direction, (Color){vfx->Tint.r, vfx->Tint.g, vfx->Tint.b, vfx->Tint.a * vfx->Alpha});

		if (vfx->Additive)
		{
			EndBlendMode();
		}
	}

	if (lastPointerSet)
	{
		for (int i = 0; i < 5; i++)
		{
			float n = (float)i / 5.0f;
			Vector2 p = Vector2Lerp(lastPointerPos, GetVirtualPointer(), n);
			DrawCircleV(p, Lerp(4, 32, n), (Color){255, 255, 255, 100});
		}
	}
}

void GameQuit()
{
	ClearGameplayData();
}