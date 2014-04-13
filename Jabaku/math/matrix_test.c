#include "matrix_test.h"
#include "matrix.h"

#include "test/testmacros.h"

#define _USE_MATH_DEFINES
#include <math.h>

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
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&zero); \
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
})\
TEST(set, { \
	JBKMatrix m = JBKMatrix_Set(1.0f, 2.0f, 3.0f, 4.0f, \
		5.0f, 6.0f, 7.0f, 8.0f, \
		9.0f, 10.0f, 11.0f, 12.0f, \
		13.0f, 14.0f, 15.0f, 16.0f); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	JBKVector4 ref1 = JBKVector4_Set(5.0f, 6.0f, 7.0f, 8.0f); \
	JBKVector4 ref2 = JBKVector4_Set(9.0f, 10.0f, 11.0f, 12.0f); \
	JBKVector4 ref3 = JBKVector4_Set(13.0f, 14.0f, 15.0f, 16.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(transpose, { \
	JBKMatrix m = JBKMatrix_Set(1.0f, 2.0f, 3.0f, 4.0f, \
	5.0f, 6.0f, 7.0f, 8.0f, \
	9.0f, 10.0f, 11.0f, 12.0f, \
	13.0f, 14.0f, 15.0f, 16.0f); \
	m = JBKMatrix_Transpose(JBKMatrix_Reg(m));\
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(1.0f, 5.0f, 9.0f, 13.0f); \
	JBKVector4 ref1 = JBKVector4_Set(2.0f, 6.0f, 10.0f, 14.0f); \
	JBKVector4 ref2 = JBKVector4_Set(3.0f, 7.0f, 11.0f, 15.0f); \
	JBKVector4 ref3 = JBKVector4_Set(4.0f, 8.0f, 12.0f, 16.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(scale, { \
	JBKMatrix m = JBKMatrix_Scale(JBKVector4_Set(2.0, 3.0, 4.0, 0.0)); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(2.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 3.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 4.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(rotX90Degree, { \
	JBKMatrix m = JBKMatrix_RotationX((float)M_PI_2); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(1.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 0.0f, 1.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, -1.0f, 0.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(rotY90Degree, { \
	JBKMatrix m = JBKMatrix_RotationY((float)M_PI_2); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(0.0f, 0.0f, -1.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 1.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(1.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(rotZ90Degree, { \
	JBKMatrix m = JBKMatrix_RotationZ((float)M_PI_2); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(0.0f, 1.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(-1.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 1.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(rotAxis90Degree, { \
	JBKMatrix m = JBKMatrix_Rotation(JBKVector4_Set(1.0f, 0.0f, 0.0f, 0.0f), (float)M_PI_2); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(1.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 0.0f, 1.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, -1.0f, 0.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(translation, { \
	JBKMatrix m = JBKMatrix_Translation(JBKVector4_Set(2.0, 3.0, 4.0, 0.0)); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(1.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 1.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 1.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(2.0f, 3.0f, 4.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(look, { \
	JBKMatrix m = JBKMatrix_Look(JBKVector4_Zero(), JBKVector4_Set(0.0, 0.0, 1.0, 0.0), JBKVector4_Set(0.0, 1.0, 0.0, 0.0)); \
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
})\
TEST(lookAt, { \
	JBKMatrix m = JBKMatrix_LookAt(JBKVector4_Zero(), JBKVector4_Set(0.0, 0.0, 1.0, 0.0), JBKVector4_Set(0.0, 1.0, 0.0, 0.0)); \
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
})\
TEST(ortho, { \
	JBKMatrix m = JBKMatrix_Orthographic(1024.0f, 768.0f, 0.1f, 10.0f); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(0.00195313f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 0.002604167f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 0.10101010f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, -0.01010101f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(perspective, { \
	JBKMatrix m = JBKMatrix_Perspective((float)M_PI_2, 1.33333333f, 0.1f, 10.0f); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(m)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(m)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(m)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(m)); \
	JBKVector4 ref0 = JBKVector4_Set(0.75f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 1.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 1.01010101f, 1.0f); \
	JBKVector4 ref3 = JBKVector4_Set(0.0f, 0.0f, -0.10101010f, 0.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(mul, { \
	JBKMatrix m1 = JBKMatrix_Scale(JBKVector4_Set(2.0f, 3.0f, 4.0f, 1.0f)); \
	JBKMatrix m2 = JBKMatrix_Translation(JBKVector4_Set(1.0f, 2.0f, 3.0f, 1.0f)); \
	JBKMatrix res = JBKMatrix_Mul(JBKMatrix_Reg(m1), JBKMatrix_Reg(m2)); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(res)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(res)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(res)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(res)); \
	JBKVector4 ref0 = JBKVector4_Set(2.0f, 0.0f, 0.0f, 0.0f); \
	JBKVector4 ref1 = JBKVector4_Set(0.0f, 3.0f, 0.0f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.0f, 4.0f, 0.0f); \
	JBKVector4 ref3 = JBKVector4_Set(1.0f, 2.0f, 3.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&r0, (float*)&ref0); \
	ASSERT_EQUAL_FLOAT4((float*)&r1, (float*)&ref1); \
	ASSERT_EQUAL_FLOAT4((float*)&r2, (float*)&ref2); \
	ASSERT_EQUAL_FLOAT4((float*)&r3, (float*)&ref3); \
})\
TEST(determinant, { \
	JBKMatrix m = JBKMatrix_Set(\
		1.0f, 2.0f, 3.0f, 4.0f, \
		3.0f, 1.0f, 2.0f, 3.0f, \
		5.0f, 3.0f, 1.0f, 2.0f, \
		7.0f, 5.0f, 3.0f, 1.0f); \
	JBKScalar res = JBKMatrix_Determinant(JBKMatrix_Reg(m)); \
	JBKScalar ref = JBKScalar_Set(-81.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(inverse, { \
	JBKMatrix m = JBKMatrix_Set(\
		1.0f, 2.0f, 3.0f, 4.0f, \
		3.0f, 1.0f, 2.0f, 3.0f, \
		5.0f, 3.0f, 1.0f, 2.0f, \
		7.0f, 5.0f, 3.0f, 1.0f); \
	JBKScalar det; \
	JBKMatrix res = JBKMatrix_Inverse(JBKMatrix_Reg(m), &det); \
	JBKVector4 r0 = JBKMatrix_GetR0(JBKMatrix_Reg(res)); \
	JBKVector4 r1 = JBKMatrix_GetR1(JBKMatrix_Reg(res)); \
	JBKVector4 r2 = JBKMatrix_GetR2(JBKMatrix_Reg(res)); \
	JBKVector4 r3 = JBKMatrix_GetR3(JBKMatrix_Reg(res)); \
	JBKVector4 ref0 = JBKVector4_Set(-0.259259259f, 0.333333333f, 0.0f, 0.037037037f); \
	JBKVector4 ref1 = JBKVector4_Set(0.333333333f, -0.666666667f, 0.333333333f, 0.0f); \
	JBKVector4 ref2 = JBKVector4_Set(0.0f, 0.333333333f, -0.666666667f, 0.333333333f); \
	JBKVector4 ref3 = JBKVector4_Set(0.148148148f, 0.0f, 0.333333333f, -0.259259259f); \
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
