#include "screen.h"

static RenderTexture2D screenRT;
static float holdTimer;
static bool firedHold;

static void GetRenderScaleAndOffset(int *offsetX, int *offsetY, float *scale)
{
    int realW = GetScreenWidth();
    int realH = GetScreenHeight();

    *scale = fminf((float)realW / VIRTUAL_WIDTH, (float)realH / VIRTUAL_HEIGHT);
    *offsetX = (realW - (int)(VIRTUAL_WIDTH * (*scale))) / 2;
    *offsetY = (realH - (int)(VIRTUAL_HEIGHT * (*scale))) / 2;
}

void InitScreen(void)
{
    screenRT = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    SetTextureFilter(screenRT.texture, TEXTURE_FILTER_POINT);
}

void BeginScreen(void)
{
    BeginTextureMode(screenRT);
    ClearBackground(BLACK);
}

void EndScreen(void)
{
    EndTextureMode();
}

void DrawScreen(void)
{
    int offsetX, offsetY;
    float scale;
    GetRenderScaleAndOffset(&offsetX, &offsetY, &scale);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(
        screenRT.texture,
        (Rectangle){0, 0, (float)VIRTUAL_WIDTH, -(float)VIRTUAL_HEIGHT},
        (Rectangle){offsetX, offsetY, VIRTUAL_WIDTH * scale, VIRTUAL_HEIGHT * scale},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    DrawFPS(4, 4);
    EndDrawing();
}

void DisposeScreen(void)
{
    UnloadRenderTexture(screenRT);
}

Vector2 GetVirtualPointer(void)
{
    int offsetX, offsetY;
    float scale;
    GetRenderScaleAndOffset(&offsetX, &offsetY, &scale);

    Vector2 pos = {-1, -1};

    if (GetTouchPointCount() > 0)
        pos = GetTouchPosition(0);
    else
        pos = GetMousePosition();

    if (pos.x >= 0 && pos.y >= 0)
    {
        pos.x = (pos.x - offsetX) / scale;
        pos.y = (pos.y - offsetY) / scale;
    }

    return pos;
}

Vector2 GetRealPointerFromVirtual(Vector2 virtualPos)
{
    int offsetX, offsetY;
    float scale;
    GetRenderScaleAndOffset(&offsetX, &offsetY, &scale);

    return (Vector2){
        virtualPos.x * scale + offsetX,
        virtualPos.y * scale + offsetY};
}

bool IsPointerHold(float dt)
{
    bool hold = IsMouseButtonDown(MOUSE_BUTTON_LEFT) || GetTouchPointCount() > 0;
    if (!hold)
    {
        firedHold = false;
        holdTimer = 0;
        return false;
    }

    if (firedHold)
        return false;

    holdTimer += dt;

    if (holdTimer >= 0.5f)
    {
        firedHold = true;
        TraceLog(LOG_INFO, "%.2f", holdTimer);
        holdTimer = 0;
        return true;
    }

    return false;
}

bool IsPointerDown(void)
{
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT) || GetTouchPointCount() > 0;
}

bool IsPointerPressed(void)
{
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || GetTouchPointCount() > 0;
}

Vector3 Vector3Snap(Vector3 input)
{
    return (Vector3){roundf(input.x), roundf(input.y), roundf(input.z)};
}
