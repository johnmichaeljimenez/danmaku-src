#include "ui.h"

EMSCRIPTEN_KEEPALIVE void OnButtonClick(const char* buttonID, int data)
{
	TraceLog(LOG_INFO, "ID: %s CLICK: %d", buttonID, data);
}

void InitJSUI()
{
	emscripten_run_script("resizeUI();");
}
