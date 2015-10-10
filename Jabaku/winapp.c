#include "winapp.h"

#include <string.h>
#include <sdl.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "sdl2main.lib")

#include "memory/allocmacros.h"
#include "utility/assertion.h"

#define ASSERT_SDL(condition) ASSERT(condition, SDL_GetError())

JBKWinAppResult JBKWinApp_Init(JBKWinApp* inst, uint32_t width, uint32_t height, const char* title, const JBKWinAppWindowType type) {
	ASSERT_SDL(SDL_Init(SDL_INIT_EVERYTHING) != -1);

	SDL_Window *win = NULL;
	Uint32 flags = 0;
	switch (type) {
	case JBK_WINAPP_WINDOWED: flags |= SDL_WINDOW_RESIZABLE; break;
	case JBK_WINAPP_FULLSCREEN: flags |= SDL_WINDOW_FULLSCREEN; break;
	}
	win = SDL_CreateWindow(title, 100, 100, width, height, flags);
	ASSERT_SDL(win != NULL);

	strcpy_s(inst->mTitle, sizeof(inst->mTitle), title);
	inst->mWidth = width;
	inst->mHeight = height;
	inst->mPosX = 0;
	inst->mPosY = 0;
	inst->mWindow = win;
	inst->mHandle = GetActiveWindow();
	inst->mRunning = 1;

	return JBK_WINAPP_OK;
}

JBKWinAppResult JBKWinApp_Destroy(JBKWinApp* inst) {
	SDL_DestroyWindow(inst->mWindow);
	SDL_Quit();

	return JBK_WINAPP_OK;
}

// accessors
int8_t JBKWinApp_GetRunning(JBKWinApp* app) {
	return app->mRunning;
}

void* JBKWinApp_GetHandle(JBKWinApp* app) {
	return app->mHandle;
}

const char* JBKWinApp_GetTitle(JBKWinApp* app) {
	return app->mTitle;
}

JBKWinSize JBKWinApp_GetSize(JBKWinApp* inst) {
	JBKWinSize result = { 0 };
	SDL_GetWindowSize(inst->mWindow, &result.x, &result.y);
	return result;
}

// manipulators
void JBKWinApp_SetRunning(JBKWinApp* app, int8_t running) {
	app->mRunning = running;
}

void JBKWinApp_SetTitle(JBKWinApp* app, const char* title) {
    strncpy_s(app->mTitle, JBK_WINAPP_TITLESIZE, title, JBK_WINAPP_TITLESIZE);
}

SDL_Event JBKWinApp_Update(JBKWinApp* app) {
	SDL_Event e;
	while (SDL_PollEvent(&e)){
		switch (e.type) {
		case SDL_QUIT:
			app->mRunning = 0;
		}
	}

	return e;
}
