#include "main.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (player.IsAlive)
        {
            Vector2 inputMovement = Vector2Zero();
            bool isShooting = IsKeyDown(KEY_SPACE);

            if (IsKeyDown(KEY_A))
                inputMovement.x = -1;
            else if (IsKeyDown(KEY_D))
                inputMovement.x = 1;

            if (IsKeyDown(KEY_W))
                inputMovement.y = -1;
            else if (IsKeyDown(KEY_S))
                inputMovement.y = 1;

            if (isShooting)
            {
                player.FireTimer += dt;
                if (player.FireTimer >= player.FireRate)
                {
                    player.FireTimer = 0;
                    SpawnBullet(player.Position, 90, 2048, 32, true, BulletPattern_Straight);
                }
            }
            else
            {
                player.FireTimer = 0;
            }

            if (Vector2LengthSqr(inputMovement) > 0)
                inputMovement = Vector2Normalize(inputMovement);

            player.Position.x += inputMovement.x * dt * player.MovementSpeed;
            player.Position.y += inputMovement.y * dt * player.MovementSpeed;

            player.Position = Vector2Clamp(player.Position, (Vector2){0, 0}, (Vector2){VIRTUAL_WIDTH, VIRTUAL_HEIGHT});
        }

        if (IsKeyPressed(KEY_Q))
        {
            SpawnEnemy((Vector2){VIRTUAL_WIDTH * 0.5, 10}, -90, 128, 64, 10, EnemyMovementPattern_StraightDown, EnemyAttackPattern_Aimed);
        }

        for (int i = 0; i < BULLET_COUNT; i++)
        {
            Bullet *b = &bullets[i];
            if (!b->IsAlive)
                continue;

            b->Pattern(b, dt);

            if (b->Position.x < -100 || b->Position.x > VIRTUAL_WIDTH + 100 || b->Position.y < -100 || b->Position.y > VIRTUAL_HEIGHT + 100)
            {
                b->IsAlive = false;
            }
            else
            {
                if (b->FromPlayer)
                {
                    for (int i = 0; i < ENEMY_COUNT; i++)
                    {
                        Enemy *e = &enemies[i];
                        if (!e->IsAlive)
                            continue;

                        if (CheckCollisionCircles(e->Position, e->Size, b->Position, b->Size))
                        {
                            e->HP--;
                            b->IsAlive = false;
                            break;
                        }
                    }
                }
                else
                {
                    if (CheckCollisionCircles(player.Position, player.HurtboxSize, b->Position, b->Size))
                    {
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

            e->MovementPattern(e, dt);

            if (e->AttackPattern != NULL)
                e->AttackPattern(e, dt);

            if (e->HP <= 0 || e->Position.x < -100 || e->Position.x > VIRTUAL_WIDTH + 100 || e->Position.y < -100 || e->Position.y > VIRTUAL_HEIGHT + 100)
            {
                e->IsAlive = false;
                continue;
            }
        }

        BeginScreen();
        {
            ClearBackground(DARKGRAY);

            for (int i = 0; i < BULLET_COUNT; i++)
            {
                Bullet *b = &bullets[i];
                if (!b->IsAlive)
                    continue;

                DrawCircleV(b->Position, b->Size, b->FromPlayer ? WHITE : RED);
            }

            for (int i = 0; i < ENEMY_COUNT; i++)
            {
                Enemy *e = &enemies[i];
                if (!e->IsAlive)
                    continue;

                DrawCircleV(e->Position, e->Size, RED);
            }

            DrawCircleV(
                player.Position,
                player.HurtboxSize,
                GREEN);

            EndScreen();
        }

        DrawScreen();
    }

    DisposeScreen();
    CloseWindow();

    return 0;
}
