#include "math/vector.h"

static JBKVector4 g_vectorOne = {1.0f, 1.0f, 1.0f, 1.0f};

static JBKVector4 g_INF = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
static JBKVector4 g_NAN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };

static JBKVector4I g_maskXYZ = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };

JBKVector4 JBKVector4_Zero() {
	return _mm_setzero_ps();
}
JBKVector4 JBKVector4_One() {
	return g_vectorOne;
}

JBKVector4 JBKVector3_Set(float x, float y, float z) {
	return _mm_set_ps(1.0f, z, y, x);
}
JBKVector4 JBKVector4_Set(float x, float y, float z, float w) {
	return _mm_set_ps(w, z, y, x);
}

JBKVector4 JBKVector4_Replicate(float value) {
	return _mm_set_ps1(value);
}

float JBKVector4_GetX(JBKVector4Reg v) {
	return _mm_cvtss_f32(v);
}
float JBKVector4_GetY(JBKVector4Reg v) {
	JBKVector4 tmp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
	return JBKVector4_GetX(tmp);
}
float JBKVector4_GetZ(JBKVector4Reg v) {
	JBKVector4 tmp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
	return JBKVector4_GetX(tmp);
}
float JBKVector4_GetW(JBKVector4Reg v) {
	JBKVector4 tmp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
	return JBKVector4_GetX(tmp);
}
JBKVector4 JBKVector4_SetX(JBKVector4Reg v, float value) {
	JBKVector4 res = _mm_set_ss(value);
	res = _mm_move_ss(v, res);
	return res;
}
JBKVector4 JBKVector4_SetY(JBKVector4Reg v, float value) {
	JBKVector4 res = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 2, 0, 1));
	res = JBKVector4_SetX(res, value);
	res = _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 2, 0, 1));
	return res;
}
JBKVector4 JBKVector4_SetZ(JBKVector4Reg v, float value) {
	JBKVector4 res = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 1, 2));
	res = JBKVector4_SetX(res, value);
	res = _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 0, 1, 2));
	return res;
}
JBKVector4 JBKVector4_SetW(JBKVector4Reg v, float value) {
	JBKVector4 res = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 2, 1, 3));
	res = JBKVector4_SetX(res, value);
	res = _mm_shuffle_ps(res, res, _MM_SHUFFLE(0, 2, 1, 3));
	return res;
}

int8_t JBKVector4_Equal(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 res = _mm_cmpeq_ps(v1, v2);
	int bits = _mm_movemask_ps(res);
	return (bits == 0xF);
}

JBKVector4 JBKVector4_Neg(JBKVector4Reg v) {
	return _mm_sub_ps(JBKVector4_Zero(), v);
}
JBKVector4 JBKVector4_Add(JBKVector4Reg v1, JBKVector4Reg v2) {
	return _mm_add_ps(v1, v2);
}
JBKVector4 JBKVector4_Sub(JBKVector4Reg v1, JBKVector4Reg v2) {
	return _mm_sub_ps(v1, v2);
}
JBKVector4 JBKVector4_Mul(JBKVector4Reg v, JBKScalarReg s) {
	return _mm_mul_ps(s, v);
}
JBKVector4 JBKVector4_Div(JBKVector4Reg v, JBKScalarReg s) {
	JBKVector4 reciprocal = _mm_div_ps(JBKVector4_One(), s);
	return _mm_mul_ps(reciprocal, v);
}
JBKScalar JBKVector4_Dot(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 tmp2 = v2;
	JBKVector4 tmp = _mm_mul_ps(v1, tmp2);
	tmp2 = _mm_shuffle_ps(tmp2, tmp, _MM_SHUFFLE(1, 0, 0, 0));
	tmp2 = _mm_add_ps(tmp2, tmp);
	tmp = _mm_shuffle_ps(tmp, tmp2, _MM_SHUFFLE(0, 3, 0, 0));
	tmp = _mm_add_ps(tmp, tmp2);
	return _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(2, 2, 2, 2));
}

JBKScalar JBKVector3_Dot(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 res = _mm_mul_ps(v1, v2);
	JBKVector4 tmp = _mm_shuffle_ps(res, res, _MM_SHUFFLE(2, 1, 2, 1));
	res = _mm_add_ss(res, tmp);
	tmp = _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(1, 1, 1, 1));
	res = _mm_add_ss(res, tmp);
	return _mm_shuffle_ps(res, res, _MM_SHUFFLE(0, 0, 0, 0));
}
JBKVector4 JBKVector3_Cross(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 tmp1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
	JBKVector4 tmp2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));

	JBKVector4 res = _mm_mul_ps(tmp1, tmp2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(3, 0, 2, 1));
	tmp2 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3, 1, 0, 2));
	tmp1 = _mm_mul_ps(tmp1, tmp2);
	res = _mm_sub_ps(res, tmp1);
	res = _mm_and_ps(res, g_maskXYZ.v);
	res = JBKVector4_SetW(res, 1.0f);
	return res;
}

JBKScalar JBKVector3_SquareLength(JBKVector4Reg v) {
	return JBKVector3_Dot(v, v);
}
JBKScalar JBKVector4_SquareLength(JBKVector4Reg v) {
	return JBKVector4_Dot(v, v);
}
JBKScalar JBKVector3_Length(JBKVector4Reg v) {
	JBKVector4 lenSquare = JBKVector3_SquareLength(v);
	return _mm_sqrt_ps(lenSquare);
}
JBKScalar JBKVector4_Length(JBKVector4Reg v);
JBKVector4 JBKVector3_Normalize(JBKVector4Reg v) {
	JBKVector4 lenSquare = JBKVector3_SquareLength(v);

	JBKVector4 res = JBKVector3_Length(v);
	JBKVector4 zeroMask = JBKVector4_Zero();
	zeroMask = _mm_cmpneq_ps(zeroMask, res);
	lenSquare = _mm_cmpneq_ps(lenSquare, g_INF);
	res = _mm_div_ps(v, res);
	res = _mm_and_ps(res, zeroMask);
	JBKVector4 tmp1 = _mm_andnot_ps(lenSquare, g_NAN);
	JBKVector4 tmp2 = _mm_and_ps(res, lenSquare);
	res = _mm_or_ps(tmp1, tmp2);
	res = JBKVector4_SetW(res, 1.0f);
	return res;
}
JBKVector4 JBKVector4_Normalize(JBKVector4Reg v);
