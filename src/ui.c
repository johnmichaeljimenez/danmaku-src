#include "ui.h"

EMSCRIPTEN_KEEPALIVE void OnButtonClick(const char* buttonID, int data)
{
	TraceLog(LOG_INFO, "ID: %s CLICK: %d", buttonID, data);

	if (TextIsEqual(buttonID, "btnMenuNewGame"))
	{
		GoToGame(0);
	}
}

void InitJSUI()
{
	emscripten_run_script("resizeUI();");
}


void ShowScreen(const char* id)
{
	emscripten_run_script(TextFormat("showUIScreen(\"%s\");", id));
}
