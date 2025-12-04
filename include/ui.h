#pragma once
#include "main.h"

EMSCRIPTEN_KEEPALIVE void OnButtonClick(const char* buttonID, int data);
void InitJSUI();