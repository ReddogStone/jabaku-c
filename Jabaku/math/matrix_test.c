#include "matrix_test.h"
#include "matrix.h"

#include "test/testmacros.h"

static void before() {}
static void after() {}

#define TEST_LIST \
TEST(zero, { \
	JBKMatrix m = JBKMatrix_Zero(); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 zero = JBKVector4_Zero(); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&zero);\
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&zero); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&zero); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&zero); \
})\
TEST(identity, { \
	JBKMatrix m = JBKMatrix_Identity(); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(1.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 1.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 1.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})


#define TEST(name, impl) static void name() impl
TEST_LIST
#undef TEST

void JBKMatrix_RunTests() {
#define TEST(name, impl) DO_TEST(name)
	TEST_LIST
#undef TEST
}
