#ifndef JBK_XNAMATH_VECTOR_TPL_H
#define JBK_XNAMATH_VECTOR_TPL_H

#include <xmmintrin.h>
typedef __m128 JBKVector4;
typedef __declspec(align(16)) struct JBKVector4I {
	union {
		int32_t i[4];
		JBKVector4 v;
	};
} JBKVector4I;

typedef JBKVector4 JBKVector4Reg;
typedef JBKVector4* JBKVector4Ref;

#endif // JBK_XNAMATH_VECTOR_TPL_H