#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include "testerror.h"

#define DECLARE_EQ_NEQ_PRIMITIVE(Type, TypeSuffix) \
	int AssertEqual##TypeSuffix(Type a, Type b, int line, const char* file, const char* function, const char* exp); \
	int AssertNeq##TypeSuffix(Type a, Type b, int line, const char* file, const char* function, const char* aexp, const char* bexp);

DECLARE_EQ_NEQ_PRIMITIVE(int, Int);
DECLARE_EQ_NEQ_PRIMITIVE(const void*, Ptr);

int AssertEqualStr(const char* a, const char* b, int line, const char* file, const char* function, const char* exp);
int AssertNeqStr(const char* a, const char* b, int line, const char* file, const char* function, const char* aexp, const char* bexp);
int AssertEqualFloat(const float a, const float b, int line, const char* file, const char* function, const char* exp);
int AssertNeqFloat(const float a, const float b, int line, const char* file, const char* function, const char* aexp, const char* bexp);
int AssertEqualFloat4(const float a[], const float b[], int line, const char* file, const char* function, const char* exp);
int AssertNeqFloat4(const float a[], const float b[], int line, const char* file, const char* function, const char* aexp, const char* bexp);

int AssertFailure(int line, const char* file, const char* function);

#define ASSERT_EQ(TypeSuffix, a, b) if( !AssertEqual##TypeSuffix((a), (b), __LINE__, __FILE__, __FUNCTION__, #a) ) { return; }
#define ASSERT_NEQ(TypeSuffix, a, b) if( !AssertNeq##TypeSuffix((a), (b), __LINE__, __FILE__, __FUNCTION__, #a, #b) ) { return; }

#define ASSERT_EQUAL_INT(a, b) ASSERT_EQ(Int, a, b)
#define ASSERT_NEQ_INT(a, b) ASSERT_NEQ(Int, a, b)
#define ASSERT_EQUAL_PTR(a, b) ASSERT_EQ(Ptr, a, b)
#define ASSERT_NEQ_PTR(a, b) ASSERT_NEQ(Ptr, a, b)
#define ASSERT_EQUAL_STR(a, b) ASSERT_EQ(Str, a, b)
#define ASSERT_NEQ_STR(a, b) ASSERT_NEQ(Str, a, b)
#define ASSERT_EQUAL_FLOAT(a, b) ASSERT_EQ(Float, a, b)
#define ASSERT_NEQ_FLOAT(a, b) ASSERT_NEQ(Float, a, b)
#define ASSERT_EQUAL_FLOAT4(a, b) ASSERT_EQ(Float4, a, b)
#define ASSERT_NEQ_FLOAT4(a, b) ASSERT_NEQ(Float4, a, b)

#define ASSERT_FAILURE() if( !AssertFailure(__LINE__, __FILE__, __FUNCTION__) ) { return; }

#define DO_TEST(test) before(); test(); after(); if (TESTError_Get()) return;


#endif // TEST_MACROS_H