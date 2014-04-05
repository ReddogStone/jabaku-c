#ifndef JBK_WINAPP_H
#define JBK_WINAPP_H

#include <stdint.h>

typedef enum {
	JBK_WINAPP_OK
} JBKWinAppResult;

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
JBKWinAppResult JBKWinApp_Init(JBKWinApp* inst, uint32_t width, uint32_t height, const char* title);
JBKWinAppResult JBKWinApp_Destroy(JBKWinApp* inst);

// accessors
int8_t JBKWinApp_GetRunning(JBKWinApp* app);
void* JBKWinApp_GetHandle(JBKWinApp* app);
const char* JBKWinApp_GetTitle(JBKWinApp* app);

// manipulators
void JBKWinApp_SetRunning(JBKWinApp* app, int8_t running);
void JBKWinApp_SetTitle(JBKWinApp* app, const char* title);

JBKWinAppResult JBKWinApp_Update(JBKWinApp* app);

#endif // JBK_WINAPP_H