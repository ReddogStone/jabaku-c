#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>

#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "winapp.h"
#include "renderer/renderapi.h"

int main(int argc, char* argv[]) {
	// variables
	JBKWinApp app;

	// create the windows app
	JBKWinApp_Init(&app, 1024, 768, "Test App");
	JBKRenderOptions options = {0};
	JBKRender_Init(app.mHandle, &options);

	while (app.mRunning) {
		JBKWinApp_Update(&app);
		JBKRender_ClearRT(0xFFFF0000);
		JBKRender_Present();
	}

	JBKRender_Cleanup();
	JBKWinApp_Destroy(&app);

	_CrtDumpMemoryLeaks();
}