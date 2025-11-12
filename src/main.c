#include "main.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);

    Player player = {0};
    player.IsAlive = true;
    player.FireTimer = 0;
    player.HurtboxSize = (Vector2){64, 64};
    player.MovementSpeed = 512;
    player.Position = (Vector2){VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT - 100};

    while (!WindowShouldClose())
    {
        if (player.IsAlive)
        {
            Vector2 inputMovement = Vector2Zero();

            if (IsKeyDown(KEY_A))
                inputMovement.x = -1;
            else if (IsKeyDown(KEY_D))
                inputMovement.x = 1;
                
            if (IsKeyDown(KEY_W))
                inputMovement.y = -1;
            else if (IsKeyDown(KEY_S))
                inputMovement.y = 1;

            if (Vector2LengthSqr(inputMovement) > 0)
                inputMovement = Vector2Normalize(inputMovement);

            player.Position.x += inputMovement.x * GetFrameTime() * player.MovementSpeed;
            player.Position.y += inputMovement.y * GetFrameTime() * player.MovementSpeed;

            player.Position = Vector2Clamp(player.Position, (Vector2){0, 0}, (Vector2){VIRTUAL_WIDTH, VIRTUAL_HEIGHT});
        }

        BeginScreen();
        {
            ClearBackground(DARKGRAY);

            DrawRectangle(
                player.Position.x - player.HurtboxSize.x * 0.5,
                player.Position.y - player.HurtboxSize.y * 0.5,
                player.HurtboxSize.x,
                player.HurtboxSize.y,
                GREEN);

            EndScreen();
        }
        DrawScreen();
    }

    DisposeScreen();
    CloseWindow();

    return 0;
}
