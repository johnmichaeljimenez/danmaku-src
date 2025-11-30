#pragma once

#include "main.h"

#define VIRTUAL_WIDTH 1080
#define VIRTUAL_HEIGHT 1280

void InitScreen(void);
void BeginScreen(void);
void EndScreen(void);
void DrawScreen(void);
void DisposeScreen(void);

Vector2 GetVirtualPointer(void);
Vector2 GetRealPointerFromVirtual(Vector2 virtualPos);
bool IsPointerHold(float dt);
bool IsPointerDown(void);
bool IsPointerPressed(void);

Vector3 Vector3Snap(Vector3 input);