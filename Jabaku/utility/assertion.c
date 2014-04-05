#include "utility/assertion.h"

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>

void JBKUtility_Assert(int condition, const char *file, unsigned line, const char * format, ...) {
    if (!condition) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsprintf_s(buffer, 1024, format, args);
        va_end(args);

		OutputDebugString(buffer);
		OutputDebugString("\n");
	    __debugbreak();
		exit(0);
    }
}