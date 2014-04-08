#include "utility/assertion.h"

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>

#include "test/testerror.h"

static int g_unitTestMode = 0;

void JBKUtility_Assert(int condition, const char *file, unsigned line, const char * format, ...) {
	if (!condition) {
		char buffer[1024];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 1024, format, args);
		va_end(args);

		if (g_unitTestMode) {
			TESTError_Fail("%s", buffer);
		} else {
			OutputDebugString(buffer);
			OutputDebugString("\n");
//			__debugbreak();
//			exit(0);
		}
	}
}

void JBKUtility_SetUnitTestMode(int active) {
	g_unitTestMode = active;
}
