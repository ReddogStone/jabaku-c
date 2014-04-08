#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>

#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include <SDL.h>

#include "winapp.h"
#include "renderer/renderapi.h"

#include "test/test_suite.h"

char g_buffer[1024 * 1024];

int main(int argc, char* argv[]) {
	test_main();

	// variables
	JBKWinApp app;

	// create the windows app
	JBKWinApp_Init(&app, 1024, 768, "Test App");
	JBKRenderOptions options = {0};
	JBKRender_Init(app.mHandle, &options);

	// load vertex shader
	SDL_RWops* file = SDL_RWFromFile("../lib/simple_vs.cso", "rb");
	int size = SDL_RWread(file, g_buffer, 1, sizeof(g_buffer));
	SDL_RWclose(file);
	JBKShaderInputElement elements[] = {
		{ "POSITION", JBK_FMT_XYZ },
		{ "COLOR", JBK_FMT_COLOR }
	};
	JBKVertexShader* vs = JBKRender_CreateVertexShader(g_buffer, size, elements, sizeof(elements) / sizeof(elements[0]));

	// load pixel shader
	file = SDL_RWFromFile("../lib/simple_ps.cso", "rb");
	size = SDL_RWread(file, g_buffer, 1, sizeof(g_buffer));
	SDL_RWclose(file);
	JBKPixelShader* ps = JBKRender_CreatePixelShader(g_buffer, size);

	// create geometry
	struct {
		float x;
		float y;
		float z;
		int32_t color;
	} bufferData[] = {
		{ -1.0, 0.0, 0.0, 0xFF0000FF },
		{ 1.0, 0.0, 0.0, 0x00FF00FF },
		{ 0.0, 1.0, 0.0, 0x0000FFFF }
	};
	uint32_t offsets[] = { 0, 12 };
	JBKVertexBuffer* vb = JBKRender_CreateVertexBuffer(bufferData, sizeof(bufferData));

	while (app.mRunning) {
		JBKWinApp_Update(&app);
		JBKRender_ClearRT(0xFFFF0000);

		JBKRender_SetEffect(vs, ps);
		JBKRender_SetVertexBuffer(vb, sizeof(offsets) / sizeof(offsets[0]), sizeof(bufferData[0]), offsets);
		JBKRender_DrawPrimitives(JBK_PRIMITIVE_TRIANGLE_LIST, sizeof(bufferData) / (3 * sizeof(bufferData[0])), 0);

		JBKRender_Present();
	}

	JBKRender_DestroyVertexBuffer(vb);
	JBKRender_DestroyVertexShader(vs);
	JBKRender_DestroyPixelShader(ps);
	JBKRender_Cleanup();
	JBKWinApp_Destroy(&app);

	_CrtDumpMemoryLeaks();
	return 0;
}