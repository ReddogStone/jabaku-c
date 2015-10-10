#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

#include <stdint.h>
#include <SDL.h>

#pragma comment (lib, "sdl2.lib")

#include "winapp.h"
#include "renderer/renderapi.h"
#include "math/vector.h"

#include "test/test_suite.h"

char g_buffer[1024 * 1024];

int main(int argc, char* argv[]) {
	test_main();
	JBKWinApp app;
	// create the windows app
	JBKWinApp_Init(&app, 800, 600, "Test App", JBK_WINAPP_WINDOWED);
	JBKRender_Init(JBKWinApp_GetHandle(&app), NULL);

	SDL_RWops* file = SDL_RWFromFile("../lib/simple_vs.cso", "rb");
	int size = SDL_RWread(file, g_buffer, 1, sizeof(g_buffer));
	SDL_RWclose(file);

	JBKShaderInputElement elements[] = {
		{ "POSITION", JBK_FMT_XYZ },
		{ "COLOR", JBK_FMT_COLOR }
	};
	JBKVertexShader *vs = JBKRender_CreateVertexShader(g_buffer, size, elements, sizeof(elements) / sizeof(elements[0]));

	file = SDL_RWFromFile("../lib/simple_ps.cso", "rb");
	size = SDL_RWread(file, g_buffer, 1, sizeof(g_buffer));
	SDL_RWclose(file);
	JBKPixelShader *ps = JBKRender_CreatePixelShader(g_buffer, size);

	// create a triangle using the VERTEX struct
	struct { 
		float x;
		float y;
		float z;
		int32_t color;
	} bufferData[] = {
		{  0.0f,  0.5f, 0.0f, 0xFFFF0000 },
		{  0.5f,  0.0f, 0.0f, 0xFF00FF00 },
		{ -0.5f,  0.0f, 0.0f, 0xFF0000FF },
		{  0.0f, -0.5f, 0.0f, 0xFFFF0000 },
		{ -0.5f,  0.0f, 0.0f, 0xFF0000FF },
		{  0.5f,  0.0f, 0.0f, 0xFF00FF00 }
	};

	JBKVertexBuffer* vb = JBKRender_CreateVertexBuffer(bufferData, sizeof(bufferData));

	float color[] = {0.1f, 0.2f, 0.3f, 1.0f};
	JBKConstantBuffer* cb = JBKRender_CreateConstantBuffer(sizeof(color));
	{
		void* mem = JBKRender_LockConstantBuffer(cb);
		SDL_memcpy(mem, color, sizeof(color));
		JBKRender_UnlockConstantBuffer(cb);
	}

	while (JBKWinApp_GetRunning(&app)) {
		JBKWinApp_Update(&app);

		JBKRender_ClearRT(0x003366FF);

		JBKRender_SetEffect(vs, ps);

		uint32_t offset = 0;
		uint32_t stride = 16;
		JBKRender_SetPSConstants(&cb, 1);
		JBKRender_SetVertexBuffers(&vb, 1, &stride, &offset);

		JBKRender_DrawPrimitives(JBK_PRIMITIVE_TRIANGLE_LIST, sizeof(bufferData) / sizeof(bufferData[0]), 0);

		JBKRender_Present();
	}

	JBKRender_DestroyConstantBuffer(cb);
	JBKRender_DestroyVertexBuffer(vb);
	JBKRender_DestroyVertexShader(vs);
	JBKRender_DestroyPixelShader(ps);
	JBKRender_Cleanup();
	JBKWinApp_Destroy(&app);
	return 0;
}
