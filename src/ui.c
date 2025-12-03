#include "ui.h"


EMSCRIPTEN_KEEPALIVE void OnButtonClick(const char* buttonID, int data)
{
	TraceLog(LOG_INFO, "CLICK: %d", data);
}