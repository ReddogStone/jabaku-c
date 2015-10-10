#ifdef JBK_RENDERER_OPENGL

#include "renderer/renderapi.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "utility/assertion.h"

static HGLRC glRC;
static HDC hDC;

JBKRenderResult JBKRender_Create(int64_t hWindow) {
	int32_t pixelFormat;

	static PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be 
	{ 
		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor 
		1,												// Version Number 
		PFD_DRAW_TO_WINDOW |							// Format Must Support Window 
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL 
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering 
		PFD_TYPE_RGBA,									// Request An RGBA Format 
		24,												// Select Our Color Depth 
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored 
		0,												// No Alpha Buffer 
		0,												// Shift Bit Ignored 
		0,												// No Accumulation Buffer 
		0, 0, 0, 0,										// Accumulation Bits Ignored 
		24,												// 16Bit Z-Buffer (Depth Buffer) 
		8,												// No Stencil Buffer 
		0,												// No Auxiliary Buffer 
		PFD_MAIN_PLANE,									// Main Drawing Layer 
		0,												// Reserved 
		0, 0, 0											// Layer Masks Ignored 
	};

	hDC = GetDC((HWND)hWindow);

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);

	glRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, glRC);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, 1024, 768);

	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_Destroy() {
	// make the rendering context not current  
	wglMakeCurrent(NULL, NULL);
 
	// delete the rendering context  
	wglDeleteContext(glRC);

	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_ClearRT(JBKRenderColor color) {
	GLclampf red = (GLclampf)((color & 0xFF0000) >> 16) / 256.0f;
	GLclampf green = (GLclampf)((color & 0xFF00) >> 8) / 256.0f;
	GLclampf blue = (GLclampf)((color & 0xFF)) / 256.0f;
	GLclampf alpha = (GLclampf)((color & 0xFF000000) >> 24) / 256.0f;
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepth(float depth) {
	glClearDepth(depth);

	glClear(GL_DEPTH_BUFFER_BIT);

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearStencil(int32_t stencil) {
	glClearStencil(stencil);
	glClear(GL_STENCIL_BUFFER_BIT);

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepthStencil(float depth, int32_t stencil) {
	glClearDepth(depth);
	glClearStencil(stencil);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_Clear(JBKRenderColor color, float depth, int32_t stencil) {
	GLclampf red = (GLclampf)((color & 0xFF0000) >> 16) / 256.0f;
	GLclampf green = (GLclampf)((color & 0xFF00) >> 8) / 256.0f;
	GLclampf blue = (GLclampf)((color & 0xFF)) / 256.0f;
	GLclampf alpha = (GLclampf)((color & 0xFF000000) >> 24) / 256.0f;
	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearRTDef() {
	return JBKRender_ClearRT(JBK_RENDER_DEF_CLEAR_COLOR);
}
JBKRenderResult JBKRender_ClearDepthDef() {
	return JBKRender_ClearDepth(1.0f);
}
JBKRenderResult JBKRender_ClearStencilDef() {
	return JBKRender_ClearStencil(0);
}
JBKRenderResult JBKRender_ClearDepthStencilDef() {
	return JBKRender_ClearDepthStencil(1.0f, 0);
}
JBKRenderResult JBKRender_ClearDef() {
	return JBKRender_Clear(JBK_RENDER_DEF_CLEAR_COLOR, 1.0f, 0);
}

JBKRenderResult JBKRender_Present() {
	SwapBuffers(hDC);

	return JBK_RENDER_OK;
}

//static DWORD DataTypeToFVF(JBKVertexDataType type, int32_t* stride) {
//	DWORD d3dtype = 0;
//	int32_t s = 0;
//
//	if (type & JBK_VERTEX_DATA_POSITION) {
//		d3dtype |= D3DFVF_XYZ;
//		s += 3 * sizeof(float);
//	}
//	if (type & JBK_VERTEX_DATA_COLOR) {
//		d3dtype |= D3DFVF_DIFFUSE;
//		s += sizeof(DWORD);
//	}
//
//	if (stride) {
//		*stride = s;
//	}
//
//	return d3dtype;
//}
//
//static D3DPRIMITIVETYPE PrimitiveTypeToD3D(JBKPrimitiveType type) {
//	D3DPRIMITIVETYPE map[] = { D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP };
//
//	return map[(int32_t)type];
//}

JBKRenderResult JBKRender_DrawUserPrimitives(void* data, JBKVertexDataType datatype, JBKPrimitiveType primitivetype, uint32_t primitivecount) {
	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(0.0f, 0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
	glEnd();

	return JBK_RENDER_OK;
}

#endif // JBK_RENDERER_OPENGL