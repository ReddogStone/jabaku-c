#ifdef JBK_MATH_DEFAULT

#include "math/scalar.h"

JBKScalar JBKScalar_Set(float value) {
	return (JBKScalar)value;
}

float JBKScalar_Value(JBKScalarReg scalar) {
	return (float)scalar;
}

#endif // JBK_MATH_DEFAULT
