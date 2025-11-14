#include "main.h"

TweenManager gTweenManager;
GameState gameState = GAMESTATE_MENU;

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

        switch (gameState)
        {
        case GAMESTATE_MENU:
            MenuUpdate(dt);
            break;
        case GAMESTATE_INGAME:
            GameUpdate(dt);
            break;
        case GAMESTATE_PAUSE:
            PauseUpdate(dt);
            break;

        default:
            break;
        }

        TweenManager_Update(dt);
        BeginScreen();
        {
            ClearBackground(BLACK);

            switch (gameState)
            {
            case GAMESTATE_MENU:
                MenuRender(dt);
                break;
            case GAMESTATE_INGAME:
                GameRender(dt);
                break;
            case GAMESTATE_PAUSE:
                GameRender(dt);
                PauseRender(dt);
                break;

            default:
                break;
            }

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
    gameState = GAMESTATE_MENU;
    TweenManager_Clear();
    GameQuit();
    MenuStart();
}

void GoToGame(int level)
{
    gameState = GAMESTATE_INGAME;
    TweenManager_Clear();
    MenuCleanup();
    GameStart(level);
}

void PauseGame()
{
    gameState = GAMESTATE_PAUSE;
    IsGamePaused = true;
    PauseShow();
}

void UnpauseGame()
{
    PauseHide();
    gameState = GAMESTATE_INGAME;
    IsGamePaused = false;
}
