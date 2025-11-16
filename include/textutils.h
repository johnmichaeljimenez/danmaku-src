#pragma once
#include "main.h"

void GetTypedText(const char *src, float t, char *out, size_t outSize);
void DrawTextStyled(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color);