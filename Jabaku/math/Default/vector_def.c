#ifdef JBK_MATH_DEFAULT

#include "math/vector.h"

#include <math.h>

JBKVector4 JBKVector4_Zero() {
	JBKVector4 res = {0.0f, 0.0f, 0.0f, 0.0f};
	return res;
}

JBKVector4 JBKVector4_One() {
	JBKVector4 res = {1.0f, 1.0f, 1.0f, 1.0f};
	return res;
}

JBKVector4 JBKVector4_Set(float x, float y, float z, float w) {
	JBKVector4 res = {x, y, z, w};
	return res;
}

JBKVector4 JBKVector4_Replicate(float value) {
	JBKVector4 res = {value, value, value, value};
	return res;
}

JBKVector4 JBKVector4_Neg(JBKVector4Reg v) {
	JBKVector4 res = {-v.mX, -v.mY, -v.mZ, -v.mW};
	return res;
}

JBKVector4 JBKVector4_Add(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 res = {v1.mX + v2.mX, v1.mY + v2.mY, v1.mZ + v2.mZ, v1.mW + v2.mW};
	return res;
}

JBKVector4 JBKVector4_Sub(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 res = {v1.mX - v2.mX, v1.mY - v2.mY, v1.mZ - v2.mZ, v1.mW - v2.mW};
	return res;
}

JBKVector4 JBKVector4_Mul(JBKVector4Reg v, JBKScalarReg s) {
	JBKVector4 res = {v.mX * s, v.mY * s, v.mZ * s, v.mW * s};
	return res;
}

JBKVector4 JBKVector4_Div(JBKVector4Reg v, JBKScalarReg s) {
	return JBKVector4_Mul(v, 1.0f / s);
}

JBKScalar JBKVector4_Dot(JBKVector4Reg v1, JBKVector4Reg v2) {
	// we know that scalar is just float in that definition
	JBKScalar res = v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ + v1.mW * v2.mW;
	return res;
}

JBKVector4 JBKVector3_Set(float x, float y, float z) {
	JBKVector4 res = {x, y, z, 1.0f};
	return res;
}

JBKScalar JBKVector3_Dot(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKScalar res = v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ;
	return res;
}

JBKVector4 JBKVector3_Cross(JBKVector4Reg v1, JBKVector4Reg v2) {
	JBKVector4 res = {
		v1.mY * v2.mZ - v1.mZ * v2.mY,
		v1.mZ * v2.mX - v1.mX * v2.mZ,
		v1.mX * v2.mY - v1.mY * v2.mX,
		1.0f
	};
	return res;
}

JBKScalar JBKVector3_SquareLength(JBKVector4Reg v) {
	return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ;
}

JBKScalar JBKVector4_SquareLength(JBKVector4Reg v) {
	return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW;
}

JBKScalar JBKVector3_Length(JBKVector4Reg v) {
	return (JBKScalar)sqrt(v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ);
}

JBKScalar JBKVector4_Length(JBKVector4Reg v) {
	return (JBKScalar)sqrt(v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW);
}

JBKVector4 JBKVector3_Normalize(JBKVector4Reg v) {
	return JBKVector4_Div(v, JBKVector3_Length(v));
}

JBKVector4 JBKVector4_Normaize(JBKVector4Reg v) {
	return JBKVector4_Div(v, JBKVector4_Length(v));
}

#endif // JBK_MATH_DEFAULT