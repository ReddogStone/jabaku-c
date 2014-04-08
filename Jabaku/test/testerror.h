#ifndef TEST_ERROR_H
#define TEST_ERROR_H

void TESTError_Fail(const char * format, ...);
void TESTError_Recover();

const char* TESTError_Get();

#endif // TEST_ERROR_H