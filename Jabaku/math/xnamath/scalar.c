#include "math/scalar.h"

JBKScalar JBKScalar_Set(float value) {
	return _mm_set_ps(value, value, value, value);
}
float JBKScalar_Value(JBKScalarReg scalar) {
	return scalar.m128_f32[0];
}
