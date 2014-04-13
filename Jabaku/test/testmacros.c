#include "testmacros.h"

#include "testerror.h"

#define DEFINE_EQ_NEQ_PRIMITIVE(Type, TypeSuffix) \
	int AssertEqual##TypeSuffix(Type a, Type b, int line, const char* file, const char* function, const char* exp) {	\
		if (a != b) {																									\
			TESTError_Fail("%s:%d> %s() FAILED: %s was %d but should be %d\n", file, line, function, exp, a, b);		\
			return 0;																									\
		}																												\
																														\
		return 1;																										\
	}																													\
																														\
	int AssertNeq##TypeSuffix(Type a, Type b, int line, const char* file, const char* function, const char* aexp, const char* bexp) {	\
		if (a == b) {																													\
			TESTError_Fail("%s:%d> %s() FAILED: %s should not be %d (%s)\n", file, line, function, aexp, b, bexp);						\
			return 0;																													\
		}																																\
																																		\
		return 1;																														\
	}

DEFINE_EQ_NEQ_PRIMITIVE(int, Int)
DEFINE_EQ_NEQ_PRIMITIVE(void*, Ptr)

#include <string.h>

int AssertEqualStr(const char* a, const char* b, int line, const char* file, const char* function, const char* exp) {
	if (strcmp(a, b) != 0) {
		TESTError_Fail("%s:%d> %s() FAILED: %s was '%s' but should be '%s'\n", file, line, function, exp, a, b);
		return 0;
	}

	return 1;
}

int AssertNeqStr(const char* a, const char* b, int line, const char* file, const char* function, const char* aexp, const char* bexp) {
	if (strcmp(a, b) == 0) {
		TESTError_Fail("%s:%d> %s() FAILED: %s should not be '%s' (%s)\n", file, line, function, aexp, b, bexp);
		return 0;
	}

	return 1;
}

int AssertEqualFloat(const float a, const float b, int line, const char* file, const char* function, const char* exp) {
	if (a != b) {
		TESTError_Fail("%s:%d> %s() FAILED: %s was '%f' but should be '%f'\n", file, line, function, exp, a, b);
		return 0;
	}

	return 1;
}

int AssertNeqFloat(const float a, const float b, int line, const char* file, const char* function, const char* aexp, const char* bexp) {
	if (a == b) {
		TESTError_Fail("%s:%d> %s() FAILED: %s should not be '%f' (%s)\n", file, line, function, aexp, b, bexp);
		return 0;
	}

	return 1;
}

int AssertEqualFloat4(const float a[], const float b[], int line, const char* file, const char* function, const char* exp) {
	if ((a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]) || (a[3] != b[3])) {
		TESTError_Fail("%s:%d> %s() FAILED: %s was '(%f,%f,%f,%f)' but should be '(%f,%f,%f,%f)'\n", file, line, function, exp, 
			a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);
		return 0;
	}

	return 1;
}

int AssertNeqFloat4(const float a[], const float b[], int line, const char* file, const char* function, const char* aexp, const char* bexp) {
	if ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3])) {
		TESTError_Fail("%s:%d> %s() FAILED: %s should not be '(%f,%f,%f,%f)' (%s)\n", file, line, function, aexp, b[0], b[1], b[2], b[3], bexp);
		return 0;
	}

	return 1;
}

int AssertFailure(int line, const char* file, const char* function) {
	if (!TESTError_Get()) {
		TESTError_Fail("%s:%d> %s() FAILED: Should have failed, bud didn't\n", file, line, function);
		return 0;
	}

	TESTError_Recover();
	return 1;
}
