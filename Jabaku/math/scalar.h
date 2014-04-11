#ifndef JBK_MATH_SCALAR_H
#define JBK_MATH_SCALAR_H

#if defined(JBK_MATH_DEFAULT)
#include "math/default/scalar_tpl_def.h"
#elif defined(JBK_MATH_XNAMATH)
#include "math/xnamath/scalar_tpl.h"
#else
#pragma message("Math not implemented!")
#endif

JBKScalar JBKScalar_Set(float value);
float JBKScalar_Value(JBKScalarReg scalar);

#endif // JBK_MATH_SCALAR_H