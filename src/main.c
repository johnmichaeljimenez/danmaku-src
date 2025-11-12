#include "main.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);

    Bullet bulletPool[BULLET_COUNT];
    int bulletIndex = 0;
    int bulletCount = 0;

    Player player = {0};
    player.IsAlive = true;
    player.FireTimer = 0;
    player.FireRate = 0.1f;
    player.HurtboxSize = 48;
    player.MovementSpeed = 512;
    player.Position = (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT - 100};

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

                    Bullet *b = &bulletPool[bulletIndex];
                    b->Position = player.Position;
                    b->FromPlayer = true;
                    b->IsAlive = true;
                    b->MovementSpeed = 2048;
                    b->Size = 32;
                    b->Angle = 0;

                    bulletCount++;
                    bulletIndex++;
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

            for (int i = 0; i < BULLET_COUNT; i++)
            {
                Bullet *b = &bulletPool[i];
                if (!b->IsAlive)
                    continue;

                b->Position = Vector2Add(b->Position, (Vector2){0, -b->MovementSpeed * dt});
                if (b->Position.x < -100 || b->Position.x > VIRTUAL_WIDTH + 100 || b->Position.y < -100 || b->Position.y > VIRTUAL_HEIGHT + 100)
                {
                    b->IsAlive = false;
                    bulletIndex = i;
                    bulletCount--;
                }
            }
        }

        BeginScreen();
        {
            ClearBackground(DARKGRAY);

            for (int i = 0; i < BULLET_COUNT; i++)
            {
                Bullet *b = &bulletPool[i];
                if (!b->IsAlive)
                    continue;

                DrawCircleV(b->Position, b->Size, b->FromPlayer ? WHITE : RED);
            }

            DrawCircleV(
                player.Position,
                player.HurtboxSize,
                GREEN);

            EndScreen();
        }

        DrawText(TextFormat("Bullet index: %d\nBullet count: %d", bulletIndex, bulletCount), 4, 20, 28, RED);
        DrawScreen();
    }

    DisposeScreen();
    CloseWindow();

    return 0;
}
