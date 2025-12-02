#include "game.h"

bool IsCutscene;
int RetryCount;

Vector2 lastPointerPos;
bool lastPointerSet;

static float cutsceneTimer;
static int playerMovementState;

bool finishedGame;

static float hsDuration;
static float hsTimer;

Texture2D playerDefaultBullet;

static void OnCutsceneTimerDone(void)
{
	IsCutscene = false;
}

void OnEndLevel(void)
{
	finishedGame = LevelIndex >= LEVEL_COUNT-1;
	EndGame(player.IsAlive);
}

void ClearInput()
{
	if (playerMovementState != 0)
	{
		playerMovementState = 0;
		ReuseAnimation(player.Animation, "PlayerIdle");
	}

	lastPointerSet = false;
}

static void ClearBulletsEnd()
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (!bullets[i].IsAlive)
			continue;
		
		VFX *bvfx = SpawnVFX(bullets[i].Position, bullets[i].Animation->Clip->Frames[bullets[i].Animation->FrameIndex], bullets[i].Angle, 1.0f);
		
		TweenManager_AddFloatFrom(&bvfx->Scale, 1, 2, 0.4f, EASING_EASEOUTQUAD, TextFormat("VFX-BulletEndS-%d", i), NULL);
		TweenManager_AddFloatFrom(&bvfx->Alpha, 1, 0, 0.3f, EASING_EASEOUTQUAD, TextFormat("VFX-BulletEndA-%d", i), NULL);
		
		// if (!bullets[i].FromPlayer)
		DespawnBullet(&bullets[i]);
	}
}

static void EndLevel()
{
	ClearBulletsEnd();

	IsCutscene = true;
	TweenManager_AddFloatFrom(&cutsceneTimer, 1, 0, 2, EASING_LINEAR, "CutsceneTimer", OnCutsceneTimerDone);
	TweenManager_AddVector2(&player.Position, (Vector2){player.Position.x, -100}, 2, EASING_EASEINOUTQUAD, "PlayerTweenPosition", OnEndLevel);

	ClearInput();
}

void ClearGameplayData()
{
	memset(bullets, 0, sizeof(bullets));
	memset(vfxPool, 0, sizeof(vfxPool));
	memset(Animations, 0, sizeof(Animations));
}

void GameStart(int level)
{
	finishedGame = false;
	playerDefaultBullet = GetSprite(SPRITE_BULLET_DEFAULT);
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

	hsDuration = 0;
	hsTimer = 0;

	TweenManager_AddFloatFrom(&cutsceneTimer, 1, 0, 1, EASING_LINEAR, "CutsceneTimer", OnCutsceneTimerDone);
	TweenManager_AddVector2From(&player.Position, (Vector2){VIRTUAL_WIDTH * 0.5, VIRTUAL_HEIGHT + 100}, (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT - 100}, 1, EASING_EASEINOUTQUAD, "PlayerTweenPosition", NULL);
}

void GameUpdate(float dt)
{
	float udt = dt;

	if (hsTimer > 0)
	{
		hsTimer -= dt;
		if (hsTimer <= 0)
		{
			hsTimer = 0;
			hsDuration = 0;
		}
	}

	if (IsInputPressed(KEY_P))
	{
		DialogueSkip();
	}

	if (IsInputPressed(KEY_K))
	{
		RestartGame();
		return;
	}

	if (IsInputPressed(KEY_M))
	{
		EndLevel();
		return;
	}

	if (IsInputPressed(KEY_N))
	{
		HitPlayer();
		return;
	}

	if (IsInputPressed(KEY_B))
	{
		DialogueShow("tutorial-retry-2", NULL);
		return;
	}

	bool endedLevel = false;

	if (IsCutscene || IsDialogueActive)
	{
		ClearInput();
	}

	if (hsTimer > 0)
		dt = 0;

	UpdateLevel(dt);

	if (player.IsAlive)
	{
		if (player.ImmuneTime > 0)
			player.ImmuneTime -= dt;

		Vector2 inputMovement = Vector2Zero();

		if (!IsCutscene && !IsDialogueActive)
		{
			bool isShooting = player.ImmuneTime <= 0; // IsKeyDown(KEY_SPACE);

			if (IsInputDown(KEY_A))
				inputMovement.x = -1;
			else if (IsInputDown(KEY_D))
				inputMovement.x = 1;

			if (IsInputDown(KEY_W))
				inputMovement.y = -1;
			else if (IsInputDown(KEY_S))
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
					inputMovement = Vector2Scale(dir, 0.7f);

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
					SpawnBullet((Vector2){player.Position.x - 24, player.Position.y}, 90, true, "bullet_player_basic", NULL);
					SpawnBullet((Vector2){player.Position.x + 24, player.Position.y}, 90, true, "bullet_player_basic", NULL);
					
					PlaySFXVaried(SFX_PLAYER_FIRE, 0.05f, 1.0f);
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

		// b->Type->Pattern(b, dt);
		UpdateBullet(b, dt);

		if (!b->IsRoot && (b->Position.x < -100 || b->Position.x > VIRTUAL_WIDTH + 100 || b->Position.y < -100 || b->Position.y > VIRTUAL_HEIGHT + 100))
		{
			DespawnBullet(b);
		}
		else
		{
			if (b->FromPlayer && player.IsAlive)
			{
				for (int i = 0; i < BULLET_COUNT; i++)
				{
					Bullet *e = &bullets[i];
					if (!e->IsAlive || e->IgnoreHit || e->Type->HP == 0 || e->HP == 0)
						continue;

					if (CheckCollisionCircles(e->Position, e->Type->Size, b->Position, b->Type->Size))
					{
						e->HP--;
						DespawnBullet(b);

						VFX *vfx = SpawnVFX(e->Position, e->Animation->Clip->Frames[e->Animation->FrameIndex], 0, 0.2f);
						vfx->Additive = true;

						if (e->HP == 0)
						{
							DespawnBullet(e);

							if (e->Type->IsBoss)
							{
								VFX *vfx = SpawnVFX(e->Position, e->Animation->Clip->Frames[e->Animation->FrameIndex], 0, 1.0);
								vfx->Additive = true;

								TweenManager_AddFloatFrom(&vfx->Scale, 1, 10, 0.8f, EASING_EASEOUTQUAD, "VFX-BossDeath1", NULL);
								TweenManager_AddFloatFrom(&vfx->Alpha, 1, 0, 0.6f, EASING_EASEOUTQUAD, "VFX-BossDeath2", NULL);

								SetHitstop(1.0f);

						        PlaySFX(SFX_BOSS_DEFEAT);
								endedLevel = true;
							}
							else
							{
								TweenManager_AddFloatFrom(&vfx->Alpha, 0.2, 0, vfx->Lifetime, EASING_EASEINQUAD, TextFormat("VFX-Enemy-%d-1", vfx->ID), NULL);
								TweenManager_AddFloatFrom(&vfx->Scale, 1, 2, vfx->Lifetime, EASING_EASEINQUAD, TextFormat("VFX-Enemy-%d-2", vfx->ID), NULL);
							}
						}
						else
						{
							TweenManager_AddFloatFrom(&vfx->Alpha, 0.2, 0, vfx->Lifetime, EASING_EASEINQUAD, TextFormat("VFX-Enemy-%d-1", vfx->ID), NULL);
							TweenManager_AddFloatFrom(&vfx->Scale, 1, 2, vfx->Lifetime, EASING_EASEINQUAD, TextFormat("VFX-Enemy-%d-2", vfx->ID), NULL);
						}

						break;
					}
				}
			}
			else if (player.ImmuneTime <= 0 && player.IsAlive)
			{
				if (CheckCollisionCircles(player.Position, player.HurtboxSize, b->Position, b->Type->Size))
				{
					HitPlayer();

					if (b->Type->HP == 0)
						DespawnBullet(b);

					break;
				}
			}
		}
	}

	if (endedLevel)
	{
		ClearBulletsEnd();

		if (CurrentLevel->EndDialogue != NULL && CurrentLevel->EndDialogue != "")
			DialogueShow(CurrentLevel->EndDialogue, EndLevel);
		else
			EndLevel();
	}

	UpdateAnimations(dt);

	for (int i = 0; i < VFX_COUNT; i++)
	{
		VFX *vfx = &vfxPool[i];
		if (!vfx->IsAlive)
			continue;

		vfx->Timer += udt;
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

		DrawSprite(
			b->Type->HasAnimation?	b->Animation->Clip->Frames[b->Animation->FrameIndex] : b->Frame,
			b->Position, b->Type->FixedRotation? 0 : b->Angle, b->IgnoreHit? DARKGRAY : WHITE
		);
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

void SetHitstop(float duration)
{
	if (hsDuration > duration)
		return;

	hsDuration = duration;
	hsTimer = duration;
}