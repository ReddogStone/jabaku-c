#include "testerror.h"

#include <stdio.h>
#include <stdarg.h>

static int s_error = 0;
static char s_errorMessage[1024];

void TESTError_Fail(const char * format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf_s(s_errorMessage, 1024, format, args);
    va_end(args);

	s_error = 1;
}

void TESTError_Recover() {
	s_error = 0;
}

const char* TESTError_Get() {
	if (s_error) {
		return s_errorMessage;
	} else {
		return 0;
	}
}
