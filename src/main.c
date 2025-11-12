#include "main.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Danmaku");
    InitScreen();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginScreen();
        ClearBackground(DARKGRAY);

        

        EndScreen();
        DrawScreen();
    }

    DisposeScreen();
    CloseWindow();

    return 0;
}
