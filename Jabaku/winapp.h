#ifndef JBK_WINAPP_H
#define JBK_WINAPP_H

#include <stdint.h>

typedef enum {
	JBK_WINAPP_OK
} JBKWinAppResult;

typedef enum {
	JBK_WINAPP_WINDOWED,
	JBK_WINAPP_FULLSCREEN,
} JBKWinAppWindowType;

typedef union SDL_Event SDL_Event;

#define JBK_WINAPP_TITLESIZE 256

typedef struct JBKWinApp {
	int8_t mRunning;

    void* mWindow;
	void* mHandle;
    char mTitle[JBK_WINAPP_TITLESIZE];
    int32_t mPosX, mPosY;
    uint32_t mWidth, mHeight;
} JBKWinApp;

// constructor/destructor
JBKWinAppResult JBKWinApp_Init(JBKWinApp* inst, uint32_t width, uint32_t height, const char* title, const JBKWinAppWindowType type);
JBKWinAppResult JBKWinApp_Destroy(JBKWinApp* inst);

typedef struct JBKWinSize {
	uint32_t x;
	uint32_t y;
} JBKWinSize;

// accessors
int8_t JBKWinApp_GetRunning(JBKWinApp* app);
void* JBKWinApp_GetHandle(JBKWinApp* app);
const char* JBKWinApp_GetTitle(JBKWinApp* app);
JBKWinSize JBKWinApp_GetSize(JBKWinApp* inst);

// manipulators
void JBKWinApp_SetRunning(JBKWinApp* app, int8_t running);
void JBKWinApp_SetTitle(JBKWinApp* app, const char* title);

SDL_Event JBKWinApp_Update(JBKWinApp* app);

#endif // JBK_WINAPP_H