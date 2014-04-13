#include "vector_test.h"
#include "vector.h"

#include "test/testmacros.h"

static void before() {}
static void after() {}

#define TEST_LIST \
TEST(equal, { \
	JBKVector4 v1 = JBKVector4_Set(-1.0f, 1.0f, 15.0f, 2.0f); \
	JBKVector4 v2 = JBKVector4_Set(-1.0f, 1.0f, 15.0f, 2.0f); \
	int8_t res = JBKVector4_Equal(v1, v2); \
	ASSERT_EQUAL_INT(res, 1); \
})\
TEST(notEqual, { \
	JBKVector4 v1 = JBKVector4_Set(-1.0f, 1.0f, 15.0f, 2.0f); \
	JBKVector4 v2 = JBKVector4_Set(-1.0f, 2.0f, 15.0f, 2.0f); \
	int8_t res = JBKVector4_Equal(v1, v2); \
	ASSERT_EQUAL_INT(res, 0); \
})\
TEST(getters, { \
	JBKVector4 v = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	float vx = JBKVector4_GetX(v);\
	float vy = JBKVector4_GetY(v); \
	float vz = JBKVector4_GetZ(v); \
	float vw = JBKVector4_GetW(v); \
	ASSERT_EQUAL_FLOAT(vx, 1.0f); \
	ASSERT_EQUAL_FLOAT(vy, 2.0f); \
	ASSERT_EQUAL_FLOAT(vz, 3.0f); \
	ASSERT_EQUAL_FLOAT(vw, 4.0f); \
})\
TEST(setters, { \
	JBKVector4 v = JBKVector4_Zero(); \
	v = JBKVector4_SetX(v, 1.0f); \
	v = JBKVector4_SetY(v, 2.0f); \
	v = JBKVector4_SetZ(v, 3.0f); \
	v = JBKVector4_SetW(v, 4.0f); \
	ASSERT_EQUAL_FLOAT(JBKVector4_GetX(v), 1.0f); \
	ASSERT_EQUAL_FLOAT(JBKVector4_GetY(v), 2.0f); \
	ASSERT_EQUAL_FLOAT(JBKVector4_GetZ(v), 3.0f); \
	ASSERT_EQUAL_FLOAT(JBKVector4_GetW(v), 4.0f); \
})\
TEST(zeroAdd, { \
	JBKVector4 zero = JBKVector4_Zero(); \
	JBKVector4 v = JBKVector4_Set(-1.0f, 1.0f, 15.0f, 2.0f); \
	JBKVector4 res = JBKVector4_Add(zero, v); \
	ASSERT_EQUAL_FLOAT4((float*)&v, (float*)&res); \
})\
TEST(zeroMul, { \
	JBKVector4 zero = JBKVector4_Zero(); \
	JBKScalar s = JBKScalar_Set(10.0f); \
	JBKVector4 res = JBKVector4_Mul(zero, s); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&zero); \
})\
TEST(oneMul, { \
	JBKVector4 one = JBKVector4_One(); \
	JBKVector4 ten = JBKVector4_Set(10.0f, 10.0f, 10.0f, 10.0f); \
	JBKScalar s = JBKScalar_Set(10.0f); \
	JBKVector4 res = JBKVector4_Mul(one, s); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ten); \
})\
TEST(add, {\
	JBKVector4 v1 = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f);\
	JBKVector4 v2 = JBKVector4_Set(-1.0f, 2.0f, 1.0f, 5.0f);\
	JBKVector4 res = JBKVector4_Add(v1, v2);\
	JBKVector4 ref = JBKVector4_Set(0.0f, 4.0f, 4.0f, 9.0f);\
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref);\
})\
TEST(sub, { \
	JBKVector4 v1 = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	JBKVector4 v2 = JBKVector4_Set(-1.0f, 2.0f, 1.0f, 5.0f); \
	JBKVector4 res = JBKVector4_Sub(v1, v2); \
	JBKVector4 ref = JBKVector4_Set(2.0f, 0.0f, 2.0f, -1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(mul, { \
	JBKVector4 v = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	JBKScalar s = JBKScalar_Set(2.0f); \
	JBKVector4 res = JBKVector4_Mul(v, s); \
	JBKVector4 ref = JBKVector4_Set(2.0f, 4.0f, 6.0f, 8.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(div, { \
	JBKVector4 v = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	JBKScalar s = JBKScalar_Set(2.0f); \
	JBKVector4 res = JBKVector4_Div(v, s); \
	JBKVector4 ref = JBKVector4_Set(0.5f, 1.0f, 1.5f, 2.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(dot, { \
	JBKVector4 v1 = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	JBKVector4 v2 = JBKVector4_Set(-1.0f, 0.0f, 1.0f, 5.0f); \
	JBKVector4 res = JBKVector4_Dot(v1, v2); \
	JBKScalar ref = JBKScalar_Set(22.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(dot3, { \
	JBKVector4 v1 = JBKVector4_Set(1.0f, 2.0f, 3.0f, 4.0f); \
	JBKVector4 v2 = JBKVector4_Set(-1.0f, 0.0f, 1.0f, 5.0f); \
	JBKVector4 res = JBKVector3_Dot(v1, v2); \
	JBKScalar ref = JBKScalar_Set(2.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(cross, { \
	JBKVector4 v1 = JBKVector4_Set(1.0f, 0.0f, 0.0f, 1.0f); \
	JBKVector4 v2 = JBKVector4_Set(0.0f, 1.0f, 0.0f, 1.0f); \
	JBKVector4 res = JBKVector3_Cross(v1, v2); \
	JBKVector4 ref = JBKVector4_Set(0.0f, 0.0f, 1.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(normalize3, { \
	JBKVector4 v = JBKVector4_Set(10.0f, 0.0f, 0.0f, 1.0f); \
	JBKVector4 res = JBKVector3_Normalize(v); \
	JBKVector4 ref = JBKVector4_Set(1.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})\
TEST(normalize3Zero, { \
	JBKVector4 v = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	JBKVector4 res = JBKVector3_Normalize(v); \
	JBKVector4 ref = JBKVector4_Set(0.0f, 0.0f, 0.0f, 1.0f); \
	ASSERT_EQUAL_FLOAT4((float*)&res, (float*)&ref); \
})

#define TEST(name, impl) static void name() impl
TEST_LIST
#undef TEST

void JBKVector_RunTests() {
#define TEST(name, impl) DO_TEST(name)
	TEST_LIST
#undef TEST
}
