#ifndef JBK_XNAMATH_MATRIX_TPL_H
#define JBK_XNAMATH_MATRIX_TPL_H

#include <xmmintrin.h>
typedef __declspec(align(16)) struct JBKMatrix4x4 {
	union {
		JBKVector4 r[4];
		struct {
			float m_11, m_12, m_13, m_14;
			float m_21, m_22, m_23, m_24;
			float m_31, m_32, m_33, m_34;
			float m_41, m_42, m_43, m_44;
		};
		float m[4][4];
	};
} JBKMatrix;

typedef JBKMatrix* JBKMatrixReg;
typedef JBKMatrix* JBKMatrixRef;

#define JBKMatrix_Reg(m) (&m)
#define JBKMatrix_Ref(m) (&m)

#endif // JBK_XNAMATH_MATRIX_TPL_H