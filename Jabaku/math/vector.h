#ifndef JBK_MATH_VECTOR_H
#define JBK_MATH_VECTOR_H

#include <stdint.h>

#include "math/scalar.h"

#if defined(JBK_MATH_DEFAULT)
#include "math/default/vector_tpl_def.h"
#elif defined(JBK_MATH_XNAMATH)
#include "math/xnamath/vector_tpl.h"
#else
#pragma message("Math not implemented!")
#endif

//============================================
// Vector initialization
//============================================

JBKVector4 JBKVector4_Zero();
JBKVector4 JBKVector4_One();

JBKVector4 JBKVector3_Set(float x, float y, float z);
JBKVector4 JBKVector4_Set(float x, float y, float z, float w);

JBKVector4 JBKVector4_Replicate(float value);

//============================================
// Vector accessors
//============================================
float JBKVector4_GetX(JBKVector4Reg v);
float JBKVector4_GetY(JBKVector4Reg v);
float JBKVector4_GetZ(JBKVector4Reg v);
float JBKVector4_GetW(JBKVector4Reg v);
JBKVector4 JBKVector4_SetX(JBKVector4Reg v, float value);
JBKVector4 JBKVector4_SetY(JBKVector4Reg v, float value);
JBKVector4 JBKVector4_SetZ(JBKVector4Reg v, float value);
JBKVector4 JBKVector4_SetW(JBKVector4Reg v, float value);

//============================================
// Vector comparisson
//============================================
int8_t JBKVector4_Equal(JBKVector4Reg v1, JBKVector4Reg v2);

//============================================
// Vector arithmetics
//============================================

JBKVector4 JBKVector4_Neg(JBKVector4Reg v);

JBKVector4 JBKVector4_Add(JBKVector4Reg v1, JBKVector4Reg v2);
JBKVector4 JBKVector4_Sub(JBKVector4Reg v1, JBKVector4Reg v2);
JBKVector4 JBKVector4_Mul(JBKVector4Reg v, JBKScalarReg s);
JBKVector4 JBKVector4_Div(JBKVector4Reg v, JBKScalarReg s);
JBKScalar JBKVector4_Dot(JBKVector4Reg v1, JBKVector4Reg v2);

JBKScalar JBKVector3_Dot(JBKVector4Reg v1, JBKVector4Reg v2);
JBKVector4 JBKVector3_Cross(JBKVector4Reg v1, JBKVector4Reg v2);

JBKScalar JBKVector3_SquareLength(JBKVector4Reg v);
JBKScalar JBKVector4_SquareLength(JBKVector4Reg v);
JBKScalar JBKVector3_Length(JBKVector4Reg v);
JBKScalar JBKVector4_Length(JBKVector4Reg v);
JBKVector4 JBKVector3_Normalize(JBKVector4Reg v);
JBKVector4 JBKVector4_Normaize(JBKVector4Reg v);

#endif // JBK_MATH_VECTOR_H
