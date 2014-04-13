#include "math/matrix.h"

#include "utility/assertion.h"

#include <math.h>

static JBKVector4 g_vectorOne = { 1.0f, 1.0f, 1.0f, 1.0f };

static JBKVector4 g_identityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
static JBKVector4 g_identityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
static JBKVector4 g_identityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
static JBKVector4 g_identityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };

static JBKVector4 g_negativeX = { -1.0f, 1.0f, 1.0f, 1.0f };
static JBKVector4 g_negativeY = { 1.0f, -1.0f, 1.0f, 1.0f };
static JBKVector4 g_negativeZ = { 1.0f, 1.0f, -1.0f, 1.0f };
static JBKVector4 g_negativeW = { 1.0f, 1.0f, 1.0f, -1.0f };

static JBKVector4I g_maskXYZ = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
static JBKVector4I g_maskW = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
static JBKVector4I g_maskY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
static JBKVector4 g_minusOne = { -1.0f, -1.0f, -1.0f, -1.0f };


JBKMatrix JBKMatrix_Zero() {
	JBKMatrix res = { 0 };
	res.r[0] = JBKVector4_Zero();
	res.r[1] = JBKVector4_Zero();
	res.r[2] = JBKVector4_Zero();
	res.r[3] = JBKVector4_Zero();
	return res;
}
JBKMatrix JBKMatrix_Identity() {
	JBKMatrix res = { 0 };
	res.r[0] = g_identityR0;
	res.r[1] = g_identityR1;
	res.r[2] = g_identityR2;
	res.r[3] = g_identityR3;
	return res;
}
JBKMatrix JBKMatrix_Set(float v11, float v12, float v13, float v14,
	float v21, float v22, float v23, float v24,
	float v31, float v32, float v33, float v34,
	float v41, float v42, float v43, float v44) {
	JBKMatrix res = { 0 };
	res.r[0] = JBKVector4_Set(v11, v12, v13, v14);
	res.r[1] = JBKVector4_Set(v21, v22, v23, v24);
	res.r[2] = JBKVector4_Set(v31, v32, v33, v34);
	res.r[3] = JBKVector4_Set(v41, v42, v43, v44);
	return res;
}
JBKMatrix JBKMatrix_SetV(JBKVector4Reg v1, JBKVector4Reg v2, JBKVector4Reg v3, JBKVector4Ref v4) {
	JBKMatrix res = { 0 };
	res.r[0] = v1;
	res.r[1] = v2;
	res.r[2] = v3;
	res.r[3] = *v4;
	return res;
}

JBKVector4 JBKMatrix_GetR0(JBKMatrixReg m) {
	return m->r[0];
}
JBKVector4 JBKMatrix_GetR1(JBKMatrixReg m) {
	return m->r[1];
}
JBKVector4 JBKMatrix_GetR2(JBKMatrixReg m) {
	return m->r[2];
}
JBKVector4 JBKMatrix_GetR3(JBKMatrixReg m) {
	return m->r[3];
}

JBKMatrix JBKMatrix_Scale(JBKVector4Reg scaleVec3) {
	JBKVector4 r0 = JBKVector4_Zero();
	r0 = _mm_move_ss(r0, scaleVec3);
	JBKVector4 r1 = JBKVector4_Zero();
	r1 = _mm_shuffle_ps(scaleVec3, r1, _MM_SHUFFLE(0, 0, 1, 0));
	r1 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 2));
	JBKVector4 r2 = JBKVector4_Zero();
	r2 = _mm_shuffle_ps(r2, scaleVec3, _MM_SHUFFLE(0, 2, 0, 0));
	r2 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 2, 1, 0));

	JBKMatrix res = { 0 };
	res.r[0] = r0;
	res.r[1] = r1;
	res.r[2] = r2;
	res.r[3] = g_identityR3;
	return res;
}
JBKMatrix JBKMatrix_RotationX(float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	JBKVector4 sinVector = _mm_set_ss(s);
	JBKVector4 cosVector = _mm_set_ss(c);
	// (0, cos, sin, 0)
	cosVector = _mm_shuffle_ps(cosVector, sinVector, _MM_SHUFFLE(3, 0, 0, 3));

	JBKMatrix res = { 0 };
	res.r[0] = g_identityR0;
	res.r[1] = cosVector;
	// (0, sin, cos, 0)
	cosVector = _mm_shuffle_ps(cosVector, cosVector, _MM_SHUFFLE(3, 1, 2, 0));
	// (0, -sin, cos, 0)
	cosVector = _mm_mul_ps(cosVector, g_negativeY);
	res.r[2] = cosVector;
	res.r[3] = g_identityR3;
	return res;
}
JBKMatrix JBKMatrix_RotationY(float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	JBKVector4 sinVector = _mm_set_ss(s);
	JBKVector4 cosVector = _mm_set_ss(c);
	// (sin, 0, cos, 0)
	sinVector = _mm_shuffle_ps(sinVector, cosVector, _MM_SHUFFLE(3, 0, 3, 0));

	JBKMatrix res;
	res.r[2] = sinVector;
	// (cos, 0, sin, 0)
	sinVector = _mm_shuffle_ps(sinVector, sinVector, _MM_SHUFFLE(3, 0, 1, 2));
	// (cos, 0, -sin, 0)
	sinVector = _mm_mul_ps(sinVector, g_negativeZ);
	res.r[0] = sinVector;
	res.r[1] = g_identityR1;
	res.r[3] = g_identityR3;
	return res;
}
JBKMatrix JBKMatrix_RotationZ(float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	JBKVector4 sinVector = _mm_set_ss(s);
	JBKVector4 cosVector = _mm_set_ss(c);
	// (cos, sin, 0, 0)
	cosVector = _mm_unpacklo_ps(cosVector, sinVector);

	JBKMatrix res;
	res.r[0] = cosVector;
	// x = sin,y = cos,z = 0, w = 0
	cosVector = _mm_shuffle_ps(cosVector, cosVector, _MM_SHUFFLE(3, 2, 0, 1));
	// x = cos,y = -sin,z = 0, w = 0
	cosVector = _mm_mul_ps(cosVector, g_negativeX);
	res.r[1] = cosVector;
	res.r[2] = g_identityR2;
	res.r[3] = g_identityR3;
	return res;
}

JBKMatrix JBKMatrix_RotationNormal(JBKVector4 normalizedAxis, float angle) {
	JBKVector4 n0, n1;
	JBKVector4 v0, v1, v2;
	JBKVector4 r0, r1, r2;
	JBKVector4 c0, c1, c2;

	float s = sinf(angle);
	float c = cosf(angle);

	c2 = _mm_set_ps1(1.0f - c);
	c1 = _mm_set_ps1(c);
	c0 = _mm_set_ps1(s);

	n0 = _mm_shuffle_ps(normalizedAxis, normalizedAxis, _MM_SHUFFLE(3, 0, 2, 1));
	n1 = _mm_shuffle_ps(normalizedAxis, normalizedAxis, _MM_SHUFFLE(3, 1, 0, 2));

	v0 = _mm_mul_ps(c2, n0);
	v0 = _mm_mul_ps(v0, n1);

	r0 = _mm_mul_ps(c2, normalizedAxis);
	r0 = _mm_mul_ps(r0, normalizedAxis);
	r0 = _mm_add_ps(r0, c1);

	r1 = _mm_mul_ps(c0, normalizedAxis);
	r1 = _mm_add_ps(r1, v0);
	r2 = _mm_mul_ps(c0, normalizedAxis);
	r2 = _mm_sub_ps(v0, r2);

	v0 = _mm_and_ps(r0, g_maskXYZ.v);
	v1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(2, 1, 2, 0));
	v1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 3, 2, 1));
	v2 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(0, 0, 1, 1));
	v2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 0, 2, 0));

	r2 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1, 0, 3, 0));
	r2 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 3, 2, 0));

	JBKMatrix res = { 0 };
	res.r[0] = r2;

	r2 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(3, 2, 3, 1));
	r2 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 3, 0, 2));
	res.r[1] = r2;

	v2 = _mm_shuffle_ps(v2, v0, _MM_SHUFFLE(3, 2, 1, 0));
	res.r[2] = v2;

	res.r[3] = g_identityR3;
	return res;
}
JBKMatrix JBKMatrix_Rotation(JBKVector4 axis, float angle) {
	ASSERT(!JBKVector4_Equal(axis, JBKVector4_Zero()), "The axis may not be zero!");
	JBKVector4 normalizedAxis = JBKVector3_Normalize(axis);
	JBKMatrix res = JBKMatrix_RotationNormal(normalizedAxis, angle);
	return res;
}
JBKMatrix JBKMatrix_Translation(JBKVector4Reg transVec3) {
	JBKVector4 trans = JBKVector4_SetW(transVec3, 1.0f);

	JBKMatrix res;
	res.r[0] = g_identityR0;
	res.r[1] = g_identityR1;
	res.r[2] = g_identityR2;
	res.r[3] = trans;
	return res;
}

JBKMatrix JBKMatrix_Look(JBKVector4Reg pos, JBKVector4Reg dir, JBKVector4Reg up) {
	ASSERT(!JBKVector4_Equal(dir, JBKVector4_Zero()), "dir is zero");
	ASSERT(!JBKVector4_Equal(up, JBKVector4_Zero()), "up is zero");

	JBKVector4 r2 = JBKVector3_Normalize(dir);
	JBKVector4 r0 = JBKVector3_Cross(up, r2);
	r0 = JBKVector3_Normalize(r0);
	JBKVector4 r1 = JBKVector3_Cross(r2, r0);
	JBKVector4 negPos = _mm_mul_ps(pos, g_minusOne);
	JBKVector4 d0 = JBKVector3_Dot(r0, negPos);
	JBKVector4 d1 = JBKVector3_Dot(r1, negPos);
	JBKVector4 d2 = JBKVector3_Dot(r2, negPos);
	r0 = _mm_and_ps(r0, g_maskXYZ.v);
	r1 = _mm_and_ps(r1, g_maskXYZ.v);
	r2 = _mm_and_ps(r2, g_maskXYZ.v);
	d0 = _mm_and_ps(d0, g_maskW.v);
	d1 = _mm_and_ps(d1, g_maskW.v);
	d2 = _mm_and_ps(d2, g_maskW.v);
	d0 = _mm_or_ps(d0, r0);
	d1 = _mm_or_ps(d1, r1);
	d2 = _mm_or_ps(d2, r2);

	JBKMatrix res = { 0 };
	res.r[0] = d0;
	res.r[1] = d1;
	res.r[2] = d2;
	res.r[3] = g_identityR3;
	res = JBKMatrix_Transpose(JBKMatrix_Reg(res));
	return res;
}
JBKMatrix JBKMatrix_LookAt(JBKVector4Reg pos, JBKVector4Reg target, JBKVector4Reg up) {
	JBKVector4 dir = JBKVector4_Sub(target, pos);
	return JBKMatrix_Look(pos, dir, up);
}

JBKMatrix JBKMatrix_Orthographic(float width, float height, float nearZ, float farZ) {
	float fRange = 1.0f / (farZ - nearZ);
	JBKVector4 mem = {
		2.0f / width,
		2.0f / height,
		fRange,
		-fRange * nearZ
	};
	JBKVector4 values = mem;
	JBKVector4 tmp = JBKVector4_Zero();
	tmp = _mm_move_ss(tmp, values);

	JBKMatrix res = { 0 };
	res.r[0] = tmp;
	tmp = values;
	tmp = _mm_and_ps(tmp, g_maskY.v);
	res.r[1] = tmp;
	tmp = JBKVector4_Zero();
	values = _mm_shuffle_ps(values, g_identityR3, _MM_SHUFFLE(3, 2, 3, 2));
	tmp = _mm_shuffle_ps(tmp, values, _MM_SHUFFLE(2, 0, 0, 0));
	res.r[2] = tmp;
	tmp = _mm_shuffle_ps(tmp, values, _MM_SHUFFLE(3, 1, 0, 0));
	res.r[3] = tmp;
	return res;
}
JBKMatrix JBKMatrix_Perspective(float fovAngleY, float aspectXByY, float nearZ, float farZ) {
	float halfAngle = 0.5f * fovAngleY;
	float s = sinf(halfAngle);
	float c = cosf(halfAngle);
	float fRange = farZ / (farZ - nearZ);
	float height = c / s;
	JBKVector4 mem = {
		height / aspectXByY,
		height,
		fRange,
		-fRange * nearZ
	};
	JBKVector4 values = mem;
	JBKVector4 tmp = JBKVector4_Zero();
	tmp = _mm_move_ss(tmp, values);

	JBKMatrix res = { 0 };
	res.r[0] = tmp;
	tmp = values;
	tmp = _mm_and_ps(tmp, g_maskY.v);
	res.r[1] = tmp;
	tmp = _mm_setzero_ps();
	values = _mm_shuffle_ps(values, g_identityR3, _MM_SHUFFLE(3, 2, 3, 2));
	tmp = _mm_shuffle_ps(tmp, values, _MM_SHUFFLE(3, 0, 0, 0));
	res.r[2] = tmp;
	tmp = _mm_shuffle_ps(tmp, values, _MM_SHUFFLE(2, 1, 0, 0));
	res.r[3] = tmp;
	return res;
}
JBKMatrix JBKMatrix_ToFullZ();

JBKMatrix JBKMatrix_Mul(JBKMatrixReg m1, JBKMatrixReg m2) {
	JBKVector4 m2r0 = m2->r[0];
	JBKVector4 m2r1 = m2->r[1];
	JBKVector4 m2r2 = m2->r[2];
	JBKVector4 m2r3 = m2->r[3];

	JBKMatrix res;

	JBKVector4 w = m1->r[0];
	JBKVector4 x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
	JBKVector4 y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
	JBKVector4 z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

	x = _mm_mul_ps(x, m2r0);
	y = _mm_mul_ps(y, m2r1);
	z = _mm_mul_ps(z, m2r2);
	w = _mm_mul_ps(w, m2r3);

	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	res.r[0] = x;

	w = m1->r[1];
	x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2r0);
	y = _mm_mul_ps(y, m2r1);
	z = _mm_mul_ps(z, m2r2);
	w = _mm_mul_ps(w, m2r3);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	res.r[1] = x;

	w = m1->r[2];
	x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2r0);
	y = _mm_mul_ps(y, m2r1);
	z = _mm_mul_ps(z, m2r2);
	w = _mm_mul_ps(w, m2r3);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	res.r[2] = x;

	w = m1->r[3];
	x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
	y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
	z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
	w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));
	x = _mm_mul_ps(x, m2r0);
	y = _mm_mul_ps(y, m2r1);
	z = _mm_mul_ps(z, m2r2);
	w = _mm_mul_ps(w, m2r3);
	x = _mm_add_ps(x, z);
	y = _mm_add_ps(y, w);
	x = _mm_add_ps(x, y);
	res.r[3] = x;

	return res;
}

JBKScalar JBKMatrix_Determinant(JBKMatrixReg m) {
	JBKScalar res = { 0 };
	JBKMatrix_Inverse(m, &res);
	return res;
}
JBKMatrix JBKMatrix_Inverse(JBKMatrixReg m, JBKScalar* det) {
	JBKMatrix mat = JBKMatrix_Transpose(m);
	JBKVector4 v00 = _mm_shuffle_ps(mat.r[2], mat.r[2], _MM_SHUFFLE(1, 1, 0, 0));
	JBKVector4 v10 = _mm_shuffle_ps(mat.r[3], mat.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	JBKVector4 v01 = _mm_shuffle_ps(mat.r[0], mat.r[0], _MM_SHUFFLE(1, 1, 0, 0));
	JBKVector4 v11 = _mm_shuffle_ps(mat.r[1], mat.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	JBKVector4 v02 = _mm_shuffle_ps(mat.r[2], mat.r[0], _MM_SHUFFLE(2, 0, 2, 0));
	JBKVector4 v12 = _mm_shuffle_ps(mat.r[3], mat.r[1], _MM_SHUFFLE(3, 1, 3, 1));

	JBKVector4 d0 = _mm_mul_ps(v00, v10);
	JBKVector4 d1 = _mm_mul_ps(v01, v11);
	JBKVector4 d2 = _mm_mul_ps(v02, v12);

	v00 = _mm_shuffle_ps(mat.r[2], mat.r[2], _MM_SHUFFLE(3, 2, 3, 2));
	v10 = _mm_shuffle_ps(mat.r[3], mat.r[3], _MM_SHUFFLE(1, 1, 0, 0));
	v01 = _mm_shuffle_ps(mat.r[0], mat.r[0], _MM_SHUFFLE(3, 2, 3, 2));
	v11 = _mm_shuffle_ps(mat.r[1], mat.r[1], _MM_SHUFFLE(1, 1, 0, 0));
	v02 = _mm_shuffle_ps(mat.r[2], mat.r[0], _MM_SHUFFLE(3, 1, 3, 1));
	v12 = _mm_shuffle_ps(mat.r[3], mat.r[1], _MM_SHUFFLE(2, 0, 2, 0));

	v00 = _mm_mul_ps(v00, v10);
	v01 = _mm_mul_ps(v01, v11);
	v02 = _mm_mul_ps(v02, v12);
	d0 = _mm_sub_ps(d0, v00);
	d1 = _mm_sub_ps(d1, v01);
	d2 = _mm_sub_ps(d2, v02);

	v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 1, 3, 1));
	v00 = _mm_shuffle_ps(mat.r[1], mat.r[1], _MM_SHUFFLE(1, 0, 2, 1));
	v10 = _mm_shuffle_ps(v11, d0, _MM_SHUFFLE(0, 3, 0, 2));
	v01 = _mm_shuffle_ps(mat.r[0], mat.r[0], _MM_SHUFFLE(0, 1, 0, 2));
	v11 = _mm_shuffle_ps(v11, d0, _MM_SHUFFLE(2, 1, 2, 1));

	JBKVector4 v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 3, 3, 1));
	v02 = _mm_shuffle_ps(mat.r[3], mat.r[3], _MM_SHUFFLE(1, 0, 2, 1));
	v12 = _mm_shuffle_ps(v13, d1, _MM_SHUFFLE(0, 3, 0, 2));
	JBKVector4 v03 = _mm_shuffle_ps(mat.r[2], mat.r[2], _MM_SHUFFLE(0, 1, 0, 2));
	v13 = _mm_shuffle_ps(v13, d1, _MM_SHUFFLE(2, 1, 2, 1));

	JBKVector4 c0 = _mm_mul_ps(v00, v10);
	JBKVector4 c2 = _mm_mul_ps(v01, v11);
	JBKVector4 c4 = _mm_mul_ps(v02, v12);
	JBKVector4 c6 = _mm_mul_ps(v03, v13);

	v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(0, 0, 1, 0));
	v00 = _mm_shuffle_ps(mat.r[1], mat.r[1], _MM_SHUFFLE(2, 1, 3, 2));
	v10 = _mm_shuffle_ps(d0, v11, _MM_SHUFFLE(2, 1, 0, 3));
	v01 = _mm_shuffle_ps(mat.r[0], mat.r[0], _MM_SHUFFLE(1, 3, 2, 3));
	v11 = _mm_shuffle_ps(d0, v11, _MM_SHUFFLE(0, 2, 1, 2));

	v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(2, 2, 1, 0));
	v02 = _mm_shuffle_ps(mat.r[3], mat.r[3], _MM_SHUFFLE(2, 1, 3, 2));
	v12 = _mm_shuffle_ps(d1, v13, _MM_SHUFFLE(2, 1, 0, 3));
	v03 = _mm_shuffle_ps(mat.r[2], mat.r[2], _MM_SHUFFLE(1, 3, 2, 3));
	v13 = _mm_shuffle_ps(d1, v13, _MM_SHUFFLE(0, 2, 1, 2));

	v00 = _mm_mul_ps(v00, v10);
	v01 = _mm_mul_ps(v01, v11);
	v02 = _mm_mul_ps(v02, v12);
	v03 = _mm_mul_ps(v03, v13);
	c0 = _mm_sub_ps(c0, v00);
	c2 = _mm_sub_ps(c2, v01);
	c4 = _mm_sub_ps(c4, v02);
	c6 = _mm_sub_ps(c6, v03);

	v00 = _mm_shuffle_ps(mat.r[1], mat.r[1], _MM_SHUFFLE(0, 3, 0, 3));

	v10 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 2, 2));
	v10 = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(0, 2, 3, 0));
	v01 = _mm_shuffle_ps(mat.r[0], mat.r[0], _MM_SHUFFLE(2, 0, 3, 1));

	v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 3, 0));
	v11 = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(2, 1, 0, 3));
	v02 = _mm_shuffle_ps(mat.r[3], mat.r[3], _MM_SHUFFLE(0, 3, 0, 3));

	v12 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 2, 2));
	v12 = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(0, 2, 3, 0));
	v03 = _mm_shuffle_ps(mat.r[2], mat.r[2], _MM_SHUFFLE(2, 0, 3, 1));

	v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 3, 0));
	v13 = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(2, 1, 0, 3));

	v00 = _mm_mul_ps(v00, v10);
	v01 = _mm_mul_ps(v01, v11);
	v02 = _mm_mul_ps(v02, v12);
	v03 = _mm_mul_ps(v03, v13);
	JBKVector4 c1 = _mm_sub_ps(c0, v00);
	c0 = _mm_add_ps(c0, v00);
	JBKVector4 c3 = _mm_add_ps(c2, v01);
	c2 = _mm_sub_ps(c2, v01);
	JBKVector4 c5 = _mm_sub_ps(c4, v02);
	c4 = _mm_add_ps(c4, v02);
	JBKVector4 c7 = _mm_add_ps(c6, v03);
	c6 = _mm_sub_ps(c6, v03);

	c0 = _mm_shuffle_ps(c0, c1, _MM_SHUFFLE(3, 1, 2, 0));
	c2 = _mm_shuffle_ps(c2, c3, _MM_SHUFFLE(3, 1, 2, 0));
	c4 = _mm_shuffle_ps(c4, c5, _MM_SHUFFLE(3, 1, 2, 0));
	c6 = _mm_shuffle_ps(c6, c7, _MM_SHUFFLE(3, 1, 2, 0));
	c0 = _mm_shuffle_ps(c0, c0, _MM_SHUFFLE(3, 1, 2, 0));
	c2 = _mm_shuffle_ps(c2, c2, _MM_SHUFFLE(3, 1, 2, 0));
	c4 = _mm_shuffle_ps(c4, c4, _MM_SHUFFLE(3, 1, 2, 0));
	c6 = _mm_shuffle_ps(c6, c6, _MM_SHUFFLE(3, 1, 2, 0));

	JBKVector4 tmp = JBKVector4_Dot(c0, mat.r[0]);
	*det = tmp;
	tmp = _mm_div_ps(g_vectorOne, tmp);

	JBKMatrix res;
	res.r[0] = _mm_mul_ps(c0, tmp);
	res.r[1] = _mm_mul_ps(c2, tmp);
	res.r[2] = _mm_mul_ps(c4, tmp);
	res.r[3] = _mm_mul_ps(c6, tmp);
	return res;
}

JBKMatrix JBKMatrix_Transpose(JBKMatrixReg m) {
	JBKVector4 tmp1 = _mm_shuffle_ps(m->r[0], m->r[1], _MM_SHUFFLE(1, 0, 1, 0));
	JBKVector4 tmp2 = _mm_shuffle_ps(m->r[2], m->r[3], _MM_SHUFFLE(1, 0, 1, 0));
	JBKVector4 tmp3 = _mm_shuffle_ps(m->r[0], m->r[1], _MM_SHUFFLE(3, 2, 3, 2));
	JBKVector4 tmp4 = _mm_shuffle_ps(m->r[2], m->r[3], _MM_SHUFFLE(3, 2, 3, 2));

	JBKMatrix res = { 0 };
	res.r[0] = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(2, 0, 2, 0));
	res.r[1] = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3, 1, 3, 1));
	res.r[2] = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0));
	res.r[3] = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1));
	return res;
}
