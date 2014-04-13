#include "math/matrix.h"

static JBKVector4 g_identityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
static JBKVector4 g_identityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
static JBKVector4 g_identityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
static JBKVector4 g_identityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };

JBKMatrix JBKMatrix_Zero() {
	JBKMatrix res = { 0 };
	res.r[0] = JBKVector4_Zero();
	res.r[1] = JBKVector4_Zero();
	res.r[2] = JBKVector4_Zero();
	res.r[3] = JBKVector4_Zero();
	return res;
}
JBKMatrix JBKMatrix_Identity() {
	JBKMatrix res = { 0 };
	res.r[0] = g_identityR0;
	res.r[1] = g_identityR1;
	res.r[2] = g_identityR2;
	res.r[3] = g_identityR3;
	return res;
}
JBKMatrix JBKMatrix_Set(float v11, float v12, float v13, float v14,
	float v21, float v22, float v23, float v24,
	float v31, float v32, float v33, float v34,
	float v41, float v42, float v43, float v44) {
	JBKMatrix res = { 0 };
	res.r[0] = JBKVector4_Set(v11, v12, v13, v14);
	res.r[1] = JBKVector4_Set(v21, v22, v23, v24);
	res.r[2] = JBKVector4_Set(v31, v32, v33, v34);
	res.r[3] = JBKVector4_Set(v41, v42, v43, v44);
	return res;
}
JBKMatrix JBKMatrix_SetV(JBKVector4Reg v1, JBKVector4Reg v2, JBKVector4Reg v3, JBKVector4Ref v4) {
	JBKMatrix res = { 0 };
	res.r[0] = v1;
	res.r[1] = v2;
	res.r[2] = v3;
	res.r[3] = *v4;
	return res;
}

JBKVector4 JBKMatrix_GetR0(JBKMatrixReg m) {
	return m->r[0];
}
JBKVector4 JBKMatrix_GetR1(JBKMatrixReg m) {
	return m->r[1];
}
JBKVector4 JBKMatrix_GetR2(JBKMatrixReg m) {
	return m->r[2];
}
JBKVector4 JBKMatrix_GetR3(JBKMatrixReg m) {
	return m->r[3];
}
