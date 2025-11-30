#include "gameutils.h"

bool IsInputDown(KeyboardKey key)
{
	if (IsFading())
		return false;

	return IsKeyDown(key);
}

bool IsInputPressed(KeyboardKey key)
{
	if (IsFading())
		return false;

	return IsKeyPressed(key);
}

bool IsInputReleased(KeyboardKey key)
{
	return IsKeyReleased(key);
}