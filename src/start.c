#include "start.h"

bool started;

void StartInit()
{

}

void StartClick()
{
	if (IsPointerPressed())
	{
		started = true;
	}
}

void StartUpdate(float dt)
{

}

void StartRender(float dt)
{

}