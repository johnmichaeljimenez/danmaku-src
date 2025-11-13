#include "main.h"

bool IsIngame;

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);
    
    MenuStart();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (IsIngame)
            GameUpdate(dt);
        else
            MenuUpdate(dt);

        BeginScreen();
        {
            ClearBackground(BLACK);

            if (IsIngame)
                GameRender(dt);
            else
                MenuRender(dt);

            EndScreen();
        }

        DrawScreen();
    }

    DisposeScreen();
    CloseWindow();

    return 0;
}

void GoToMenu()
{
    IsIngame = false;
    GameQuit();
    MenuStart();
}

void GoToGame(int level)
{
    IsIngame = true;
    MenuCleanup();
    GameStart(level);
}