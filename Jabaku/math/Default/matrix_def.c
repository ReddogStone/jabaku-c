#ifdef JBK_MATH_DEFAULT

#include "math/matrix.h"

#include <math.h>

//============================================
// Matrix initialization
//============================================

JBKMatrix JBKMatrix_Zero() {
	JBKMatrix res = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	return res;
}

JBKMatrix JBKMatrix_Identity() {
	JBKMatrix res = {
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	};
	return res;
}

JBKMatrix JBKMatrix_Set(float v11, float v12, float v13, float v14, 
	float v21, float v22, float v23, float v24, 
	float v31, float v32, float v33, float v34, 
	float v41, float v42, float v43, float v44) {

	JBKMatrix res = {
		v11, v12, v13, v14, 
		v21, v22, v23, v24, 
		v31, v32, v33, v34, 
		v41, v42, v43, v44
	};
	return res;
}

JBKMatrix JBKMatrix_SetV(JBKVector4Reg v1, JBKVector4Reg v2, JBKVector4Reg v3, JBKVector4Ref v4) {
	JBKMatrix res = {
		v1.mX, v1.mY, v1.mZ, v1.mW,
		v2.mX, v2.mY, v2.mZ, v2.mW,
		v3.mX, v3.mY, v3.mZ, v3.mW,
		v4.mX, v4.mY, v4.mZ, v4.mW
	};

	return res;
}

//============================================
// Special matrices
//============================================

JBKMatrix JBKMatrix_Scale(JBKVector4 scaleVec3) {
	JBKMatrix res = {
		scaleVec3.mX, 0				, 0				, 0,
		0			, scaleVec3.mY	, 0				, 0,
		0			, 0				, scaleVec3.mZ	, 0,
		0			, 0				, 0				, 1
	};
	return res;
}

JBKMatrix JBKMatrix_RotationX(float angle) {
	float c = (float)cos(angle);
	float s = (float)sin(angle);

	JBKMatrix res = {
		1, 0,  0, 0,
		0, c, -s, 0,
		0, s,  c, 0,
		0, 0,  0, 1
	};

	return res;
}

JBKMatrix JBKMatrix_RotationY(float angle) {
	float c = (float)cos(angle);
	float s = (float)sin(angle);

	JBKMatrix res = {
		 c, 0, s, 0,
		 0, 1, 0, 0,
		-s, 0, c, 0,
		 0, 0, 0, 1
	};

	return res;
}

JBKMatrix JBKMatrix_RotationZ(float angle) {
	float c = (float)cos(angle);
	float s = (float)sin(angle);

	JBKMatrix res = {
		c, -s, 0, 0,
		s,  c, 0, 0,
		0,  0, 1, 0,
		0,  0, 0, 1
	};

	return res;
}

JBKMatrix JBKMatrix_Translation(JBKVector4 transVec3) {
	JBKMatrix res = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		transVec3.mX, transVec3.mY, transVec3.mZ, 1
	};

	return res;
}

JBKMatrix JBKMatrix_Look(JBKVector4Reg pos, JBKVector4Reg dir, JBKVector4Reg up) {
	JBKVector4 zAxis = JBKVector3_Normalize(dir);
	JBKVector4 xAxis = JBKVector3_Normalize(JBKVector3_Cross(up, zAxis));
	JBKVector4 yAxis = JBKVector3_Cross(zAxis, xAxis);

	JBKMatrix res = {
		xAxis.mX, yAxis.mX, zAxis.mX, 0,
		xAxis.mY, yAxis.mY, zAxis.mY, 0,
		xAxis.mZ, yAxis.mZ, zAxis.mZ, 0,
		-JBKVector3_Dot(xAxis, pos), -JBKVector3_Dot(yAxis, pos), -JBKVector3_Dot(zAxis, pos), 1
	};
	return res;
}

JBKMatrix JBKMatrix_LookAt(JBKVector4Reg pos, JBKVector4Reg target, JBKVector4Reg up) {
	return JBKMatrix_Look(pos, JBKVector4_Sub(target, pos), up);
}

JBKMatrix JBKMatrix_Orthographic(float width, float height, float nearZ, float farZ) {
	JBKMatrix res = {
		2.0f / width, 0				, 0							, 0,
		0			, 2.0f / height	, 0							, 0,
		0			, 0				, 1.0f / (farZ - nearZ)		, 0,
		0			, 0				, -nearZ / (farZ - nearZ)	, 0
	};
	return res;
}

JBKMatrix JBKMatrix_Perspective(float fovAngleY, float aspectXByY, float nearZ, float farZ) {
	float sy = (float)(1.0f / tan(0.5f * aspectXByY));
	float sx = sy * aspectXByY;
	float Q = farZ / (farZ - nearZ);
	JBKMatrix res = {
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, Q, 0,
		0, 0, -Q * nearZ, 1
	};
	return res;
}

JBKMatrix JBKMatrix_ToFullZ() {
	JBKMatrix res = {
		1, 0,  0, 0,
		0, 1,  0, 0,
		0, 0,  2, 0,
		0, 0, -1, 0
	};
	return res;
}

//============================================
// Matrix arithmetics
//============================================

JBKMatrix JBKMatrix_Add(JBKMatrixReg m1, JBKMatrixReg m2) {
	JBKMatrix res = {
		m1.m11 + m2.m11, m1.m12 + m2.m12, m1.m13 + m2.m13, m1.m14 + m2.m14,
		m1.m21 + m2.m21, m1.m22 + m2.m22, m1.m23 + m2.m23, m1.m24 + m2.m24,
		m1.m31 + m2.m31, m1.m32 + m2.m32, m1.m33 + m2.m33, m1.m34 + m2.m34,
		m1.m41 + m2.m41, m1.m42 + m2.m42, m1.m43 + m2.m43, m1.m44 + m2.m44
	};

	return res;
}

JBKMatrix JBKMatrix_Sub(JBKMatrixReg m1, JBKMatrixReg m2) {
	JBKMatrix res = {
		m1.m11 - m2.m11, m1.m12 - m2.m12, m1.m13 - m2.m13, m1.m14 - m2.m14,
		m1.m21 - m2.m21, m1.m22 - m2.m22, m1.m23 - m2.m23, m1.m24 - m2.m24,
		m1.m31 - m2.m31, m1.m32 - m2.m32, m1.m33 - m2.m33, m1.m34 - m2.m34,
		m1.m41 - m2.m41, m1.m42 - m2.m42, m1.m43 - m2.m43, m1.m44 - m2.m44
	};

	return res;
}

//--------------------------------------------
// Multiplies two matrices so that the
// resulting matrix represents a
// consecutive application of m1 followed by
// m2. So (m1 * m2) * v = m2 * (m1 * v)
// It is much easier to work that way.
//--------------------------------------------
JBKMatrix JBKMatrix_Mul(JBKMatrixReg m1, JBKMatrixReg m2) {
	JBKMatrix res = {
		m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31 + m1.m14 * m2.m41,
		m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32 + m1.m14 * m2.m42,
		m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33 + m1.m14 * m2.m43,
		m1.m11 * m2.m14 + m1.m12 * m2.m24 + m1.m13 * m2.m34 + m1.m14 * m2.m44,

		m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31 + m1.m24 * m2.m41,
		m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32 + m1.m24 * m2.m42,
		m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33 + m1.m24 * m2.m43,
		m1.m21 * m2.m14 + m1.m22 * m2.m24 + m1.m23 * m2.m34 + m1.m24 * m2.m44,

		m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31 + m1.m34 * m2.m41,
		m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32 + m1.m34 * m2.m42,
		m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33 + m1.m34 * m2.m43,
		m1.m31 * m2.m14 + m1.m32 * m2.m24 + m1.m33 * m2.m34 + m1.m34 * m2.m44,

		m1.m41 * m2.m11 + m1.m42 * m2.m21 + m1.m43 * m2.m31 + m1.m44 * m2.m41,
		m1.m41 * m2.m12 + m1.m42 * m2.m22 + m1.m43 * m2.m32 + m1.m44 * m2.m42,
		m1.m41 * m2.m13 + m1.m42 * m2.m23 + m1.m43 * m2.m33 + m1.m44 * m2.m43,
		m1.m41 * m2.m14 + m1.m42 * m2.m24 + m1.m43 * m2.m34 + m1.m44 * m2.m44,
	};

	return res;
}

JBKMatrix JBKMatrix_MulByScalar(JBKMatrixReg m, JBKScalarReg s) {
	JBKMatrix res = {
		m.m11 * s, m.m12 * s, m.m13 * s, m.m14 * s,
		m.m21 * s, m.m22 * s, m.m23 * s, m.m24 * s,
		m.m31 * s, m.m32 * s, m.m33 * s, m.m34 * s,
		m.m41 * s, m.m42 * s, m.m43 * s, m.m44 * s
	};
	return res;
}

JBKMatrix JBKMatrix_DivByScalar(JBKMatrixReg m, JBKScalarReg s) {
	return JBKMatrix_MulByScalar(m, 1.0f / s);
}

JBKScalar JBKMatrix_Determinant(JBKMatrixReg m) {
	return m.m11 * m.m22 * m.m33 * m.m44
		+ m.m11 * m.m23 * m.m34 * m.m42
		+ m.m11 * m.m24 * m.m32 * m.m43
		+ m.m12 * m.m21 * m.m34 * m.m43
		+ m.m12 * m.m23 * m.m31 * m.m44
		+ m.m12 * m.m24 * m.m33 * m.m41
		+ m.m13 * m.m21 * m.m32 * m.m44
		+ m.m13 * m.m22 * m.m34 * m.m41
		+ m.m13 * m.m24 * m.m31 * m.m42
		+ m.m14 * m.m21 * m.m33 * m.m42
		+ m.m14 * m.m22 * m.m31 * m.m43
		+ m.m14 * m.m23 * m.m32 * m.m41
		- m.m11 * m.m22 * m.m34 * m.m43
		- m.m11 * m.m23 * m.m32 * m.m44
		- m.m11 * m.m24 * m.m33 * m.m42
		- m.m12 * m.m21 * m.m33 * m.m44
		- m.m12 * m.m23 * m.m34 * m.m41
		- m.m12 * m.m24 * m.m31 * m.m43
		- m.m13 * m.m21 * m.m34 * m.m42
		- m.m13 * m.m22 * m.m31 * m.m44
		- m.m13 * m.m24 * m.m32 * m.m41
		- m.m14 * m.m21 * m.m32 * m.m43
		- m.m14 * m.m22 * m.m33 * m.m41
		- m.m14 * m.m23 * m.m31 * m.m42;
}

JBKMatrix JBKMatrix_Inverse(JBKMatrixReg m) {
	float det = JBKMatrix_Determinant(m);
	JBKMatrix res = {
		m.m22 * m.m33 * m.m44 + m.m23 * m.m34 * m.m42 + m.m24 * m.m32 * m.m43 - m.m22 * m.m34 * m.m43 - m.m23 * m.m32 * m.m44 - m.m24 * m.m33 * m.m42,
		m.m12 * m.m34 * m.m43 + m.m13 * m.m32 * m.m44 + m.m14 * m.m33 * m.m42 - m.m12 * m.m33 * m.m44 - m.m13 * m.m34 * m.m42 - m.m14 * m.m32 * m.m43,
		m.m12 * m.m23 * m.m44 + m.m13 * m.m24 * m.m42 + m.m14 * m.m22 * m.m43 - m.m12 * m.m24 * m.m43 - m.m13 * m.m22 * m.m44 - m.m14 * m.m23 * m.m42,
		m.m12 * m.m24 * m.m33 + m.m13 * m.m22 * m.m34 + m.m14 * m.m23 * m.m32 - m.m12 * m.m23 * m.m34 - m.m13 * m.m24 * m.m32 - m.m14 * m.m22 * m.m33,
		m.m21 * m.m34 * m.m43 + m.m23 * m.m31 * m.m44 + m.m24 * m.m33 * m.m41 - m.m21 * m.m33 * m.m44 - m.m23 * m.m34 * m.m41 - m.m24 * m.m31 * m.m43,
		m.m11 * m.m33 * m.m44 + m.m13 * m.m34 * m.m41 + m.m14 * m.m31 * m.m43 - m.m11 * m.m34 * m.m43 - m.m13 * m.m31 * m.m44 - m.m14 * m.m33 * m.m41,
		m.m11 * m.m24 * m.m43 + m.m13 * m.m21 * m.m44 + m.m14 * m.m23 * m.m41 - m.m11 * m.m23 * m.m44 - m.m13 * m.m24 * m.m41 - m.m14 * m.m21 * m.m43,
		m.m11 * m.m23 * m.m34 + m.m13 * m.m24 * m.m31 + m.m14 * m.m21 * m.m33 - m.m11 * m.m24 * m.m33 - m.m13 * m.m21 * m.m34 - m.m14 * m.m23 * m.m31,
		m.m21 * m.m32 * m.m44 + m.m22 * m.m34 * m.m41 + m.m24 * m.m31 * m.m42 - m.m21 * m.m34 * m.m42 - m.m22 * m.m31 * m.m44 - m.m24 * m.m32 * m.m41,
		m.m11 * m.m34 * m.m42 + m.m12 * m.m31 * m.m44 + m.m14 * m.m32 * m.m41 - m.m11 * m.m32 * m.m44 - m.m12 * m.m34 * m.m41 - m.m14 * m.m31 * m.m42,
		m.m11 * m.m22 * m.m44 + m.m12 * m.m24 * m.m41 + m.m14 * m.m21 * m.m42 - m.m11 * m.m24 * m.m42 - m.m12 * m.m21 * m.m44 - m.m14 * m.m22 * m.m41,
		m.m11 * m.m24 * m.m32 + m.m12 * m.m21 * m.m34 + m.m14 * m.m22 * m.m31 - m.m11 * m.m22 * m.m34 - m.m12 * m.m24 * m.m31 - m.m14 * m.m21 * m.m32,
		m.m21 * m.m33 * m.m42 + m.m22 * m.m31 * m.m43 + m.m23 * m.m32 * m.m41 - m.m21 * m.m32 * m.m43 - m.m22 * m.m33 * m.m41 - m.m23 * m.m31 * m.m42,
		m.m11 * m.m32 * m.m43 + m.m12 * m.m33 * m.m41 + m.m13 * m.m31 * m.m42 - m.m11 * m.m33 * m.m42 - m.m12 * m.m31 * m.m43 - m.m13 * m.m32 * m.m41,
		m.m11 * m.m23 * m.m42 + m.m12 * m.m21 * m.m43 + m.m13 * m.m22 * m.m41 - m.m11 * m.m22 * m.m43 - m.m12 * m.m23 * m.m41 - m.m13 * m.m21 * m.m42,
		m.m11 * m.m22 * m.m33 + m.m12 * m.m23 * m.m31 + m.m13 * m.m21 * m.m32 - m.m11 * m.m23 * m.m32 - m.m12 * m.m21 * m.m33 - m.m13 * m.m22 * m.m31
	};

	return JBKMatrix_DivByScalar(res, det);
}

JBKMatrix JBKMatrix_Transpose(JBKMatrixReg m) {
	JBKMatrix res = {
		m.m11, m.m21, m.m31, m.m41,
		m.m12, m.m22, m.m32, m.m42,
		m.m13, m.m23, m.m33, m.m43,
		m.m14, m.m24, m.m34, m.m44
	};
	return res;
}

//============================================
// Vector transformation
//============================================

JBKVector4 JBKMatrix_TransformVec4(JBKVector4Reg v, JBKMatrixReg m) {
	JBKVector4 res = {
		v.mX * m.m11 + v.mY * m.m21 + v.mZ * m.m31 + v.mW * m.m41,
		v.mX * m.m12 + v.mY * m.m22 + v.mZ * m.m32 + v.mW * m.m42,
		v.mX * m.m13 + v.mY * m.m23 + v.mZ * m.m33 + v.mW * m.m43,
		v.mX * m.m14 + v.mY * m.m24 + v.mZ * m.m34 + v.mW * m.m44
	};

	return res;
}

JBKVector4 JBKMatrix_TransformVec3(JBKVector4Reg v, JBKMatrixReg m) {
	// here we assume (w-component = 1) and reproject
	// at the end by dividing by the resulting w
	JBKVector4 res = {
		v.mX * m.m11 + v.mY * m.m21 + v.mZ * m.m31 + m.m41,
		v.mX * m.m12 + v.mY * m.m22 + v.mZ * m.m32 + m.m42,
		v.mX * m.m13 + v.mY * m.m23 + v.mZ * m.m33 + m.m43,
		v.mX * m.m14 + v.mY * m.m24 + v.mZ * m.m34 + m.m44
	};

	JBKVector4_Mul(res, 1.0f / v.mW);

	return res;
}

#endif
