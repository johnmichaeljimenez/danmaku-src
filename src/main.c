#include "main.h"

TweenManager gTweenManager;
GameState gameState = GAMESTATE_MENU;
int currentLevel;

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);

    TweenManager_Init();
    LoadAllSprites();
    SetupAnimationClips();

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
            if (IsDialogueActive)
                DialogueUpdate(dt);
            break;
        case GAMESTATE_PAUSE:
            PauseUpdate(dt);
            break;
        case GAMESTATE_GAME_END:
            GameEndUpdate(dt);
            break;

        default:
            break;
        }

        TweenManager_Update(dt);
        BeginScreen();
        {
            ClearBackground(DARKGRAY);

            switch (gameState)
            {
            case GAMESTATE_MENU:
                MenuRender(dt);
                break;
            case GAMESTATE_INGAME:
                GameRender(dt);
                if (IsDialogueActive)
                    DialogueRender(dt);
                break;
            case GAMESTATE_PAUSE:
                GameRender(dt);
                PauseRender(dt);
                break;
            case GAMESTATE_GAME_END:
                GameRender(dt);
                GameEndRender(dt);
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
    currentLevel = level;
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

void EndGame(bool win)
{
    gameState = GAMESTATE_GAME_END;
    IsGamePaused = false;
    GameEndShow(win);
}

void RestartGame()
{
    gameState = GAMESTATE_INGAME;
    TweenManager_Clear();
    GameStart(currentLevel);
}