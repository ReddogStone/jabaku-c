#ifndef JBK_MATH_MATRIX_H
#define JBK_MATH_MATRIX_H

//============================================
// All math functions assume right hand 
// coordinate system.
//============================================

#include "math/scalar.h"
#include "math/vector.h"

#ifdef JBK_MATH_DEFAULT
#include "math/default/matrix_tpl_def.h"
#elif defined(JBK_MATH_XNAMATH)
#include "math/xnamath/matrix_tpl.h"
#else
#pragma message("Math not implemented!")
#endif

//============================================
// Matrix initialization
//============================================

JBKMatrix JBKMatrix_Zero();
JBKMatrix JBKMatrix_Identity();
JBKMatrix JBKMatrix_Set(float v11, float v12, float v13, float v14, 
	float v21, float v22, float v23, float v24, 
	float v31, float v32, float v33, float v34, 
	float v41, float v42, float v43, float v44);
JBKMatrix JBKMatrix_SetV(JBKVector4Reg v1, JBKVector4Reg v2, JBKVector4Reg v3, JBKVector4Ref v4);

//============================================
// Accessors
//============================================
JBKVector4 JBKMatrix_GetR0(JBKMatrixReg m);
JBKVector4 JBKMatrix_GetR1(JBKMatrixReg m);
JBKVector4 JBKMatrix_GetR2(JBKMatrixReg m);
JBKVector4 JBKMatrix_GetR3(JBKMatrixReg m);

//============================================
// Special matrices
//============================================

JBKMatrix JBKMatrix_Scale(JBKVector4Reg scaleVec3);
JBKMatrix JBKMatrix_RotationX(float angle);
JBKMatrix JBKMatrix_RotationY(float angle);
JBKMatrix JBKMatrix_RotationZ(float angle);
JBKMatrix JBKMatrix_RotationNormal(JBKVector4 normalizedAxis, float angle);
JBKMatrix JBKMatrix_Rotation(JBKVector4 axis, float angle);
JBKMatrix JBKMatrix_Translation(JBKVector4Reg transVec3);

JBKMatrix JBKMatrix_Look(JBKVector4Reg pos, JBKVector4Reg dir, JBKVector4Reg up);
JBKMatrix JBKMatrix_LookAt(JBKVector4Reg pos, JBKVector4Reg target, JBKVector4Reg up);

// The following matrices transform into projection space assuming the half Z space (i.e. z in [0, 1])
// for OpenGL or other libraries requiring full Z use JBKMatrix_ToFullZ() and multiply at the end
JBKMatrix JBKMatrix_Orthographic(float width, float height, float nearZ, float farZ);
JBKMatrix JBKMatrix_Perspective(float fovAngleY, float aspectXByY, float nearZ, float farZ);
JBKMatrix JBKMatrix_ToFullZ();

//============================================
// Matrix arithmetics
//============================================

JBKMatrix JBKMatrix_Add(JBKMatrixReg m1, JBKMatrixReg m2);
JBKMatrix JBKMatrix_Sub(JBKMatrixReg m1, JBKMatrixReg m2);
JBKMatrix JBKMatrix_Mul(JBKMatrixReg m1, JBKMatrixReg m2);

JBKMatrix JBKMatrix_MulByScalar(JBKMatrixReg m, JBKScalarReg s);
JBKMatrix JBKMatrix_DivByScalar(JBKMatrixReg m, JBKScalarReg s);

JBKScalar JBKMatrix_Determinant(JBKMatrixReg m);
JBKMatrix JBKMatrix_Inverse(JBKMatrixReg m, JBKScalar* det);
JBKMatrix JBKMatrix_Transpose(JBKMatrixReg m);

//============================================
// Vector transformation
//============================================

JBKVector4 JBKMatrix_TransformVec4(JBKVector4Reg v, JBKMatrixReg m);
JBKVector4 JBKMatrix_TransformVec3(JBKVector4Reg v, JBKMatrixReg m);

#endif // JBK_MATH_MATRIX_H
