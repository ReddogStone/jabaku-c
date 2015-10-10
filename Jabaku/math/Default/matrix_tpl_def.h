#ifndef JBK_MATH_MATRIX_DEF_H
#define JBK_MATH_MATRIX_DEF_H

/**
 * We make the default matrix implementation row major.
 * So used to it from DirectX. However, nothing
 * prevents one from defining them in any other way.
 */

typedef struct JBKMatrix {
	union {
		struct {
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};
		float mMat[16];
		float mMat2D[4][4];
	};
} JBKMatrix;

typedef JBKMatrix JBKMatrixReg;
typedef JBKMatrix JBKMatrixRef;

#endif // JBK_MATH_MATRIX_DEF_H
