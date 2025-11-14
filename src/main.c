#include "main.h"

bool IsIngame;
TweenManager gTweenManager;

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);

    TweenManager_Init();
    LoadAllSprites();

    MenuStart();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (IsIngame)
            GameUpdate(dt);
        else
            MenuUpdate(dt);

        TweenManager_Update(dt);
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

    TweenManager_Clear();
    UnloadAllSprites();
    DisposeScreen();

    CloseWindow();

    return 0;
}

void GoToMenu()
{
    IsIngame = false;
    TweenManager_Clear();
    GameQuit();
    MenuStart();
}

void GoToGame(int level)
{
    IsIngame = true;
    TweenManager_Clear();
    MenuCleanup();
    GameStart(level);
}