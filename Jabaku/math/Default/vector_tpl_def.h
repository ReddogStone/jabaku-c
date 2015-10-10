#ifndef JBK_MATH_VECTOR_DEF_H
#define JBK_MATH_VECTOR_DEF_H

typedef struct JBKVector4 {
	union {
		struct {
			float mX, mY, mZ, mW;
		};
		float mV[4];
	};
} JBKVector4;

typedef JBKVector4 JBKVector4Reg;
typedef JBKVector4 JBKVector4Ref;

#endif // JBK_MATH_VECTOR_DEF_H