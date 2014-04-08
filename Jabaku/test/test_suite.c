#include "test_suite.h"

#include "testerror.h"

#include "containers/array_pool_test.h"
#include "utility/assertion.h"

#include <stdio.h>
#include <windows.h>

void test_main() {
	JBKUtility_SetUnitTestMode(1);

	const char* errorMsg = 0;

	JBKArrayList_RunTests();

	errorMsg = TESTError_Get();
	if (errorMsg) {
		printf("%s\n", errorMsg);
		system("pause");
	} else {
		printf("ALL TESTS PASSED\n");
	}

	JBKUtility_SetUnitTestMode(0);
}