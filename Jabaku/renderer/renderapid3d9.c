#ifdef JBK_RENDERER_DIRECT3D9

#include "renderer/renderapi.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "utility/assertion.h"
#include "utility/dxutils.h"

static LPDIRECT3D9 gD3D;
static LPDIRECT3DDEVICE9 gDevice;
static D3DPRESENT_PARAMETERS gPresentParams;

static LPD3DXFONT gDebugFont;

//===================================================================
// STATIC HELPERS
//===================================================================
static DWORD DataTypeToFVF(JBKVertexDataType type, int32_t* stride) {
	DWORD d3dtype = 0;
	int32_t s = 0;

	if (type & JBK_VERTEX_DATA_POSITION) {
		d3dtype |= D3DFVF_XYZ;
		s += 3 * sizeof(float);
	} else if (type & JBK_VERTEX_DATA_TRANSFORMED_POS) {
		d3dtype |= D3DFVF_XYZRHW;
		s += 4 * sizeof(float);
	}

	if (type & JBK_VERTEX_DATA_COLOR) {
		d3dtype |= D3DFVF_DIFFUSE;
		s += sizeof(DWORD);
	}
	if (type & JBK_VERTEX_DATA_TEX1) {
		d3dtype |= D3DFVF_TEX1;
		s += 2 * sizeof(float);
	} else if (type & JBK_VERTEX_DATA_TEX2) {
		d3dtype |= D3DFVF_TEX2;
		s += 4 * sizeof(float);
	}

	if (stride) {
		*stride = s;
	}

	return d3dtype;
}

static D3DPRIMITIVETYPE PrimitiveTypeToD3D(JBKPrimitiveType type) {
	D3DPRIMITIVETYPE map[] = { D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP };

	return map[(int32_t)type];
}

static void SetupTextureStageStates(DWORD d3dtype) {
	if (d3dtype & D3DFVF_TEX1) {
		IDirect3DDevice9_SetTextureStageState(gDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		IDirect3DDevice9_SetTextureStageState(gDevice, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		if (d3dtype & D3DFVF_DIFFUSE) {
			IDirect3DDevice9_SetTextureStageState(gDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		} else {
			IDirect3DDevice9_SetTextureStageState(gDevice, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		}
	} else {
		IDirect3DDevice9_SetTextureStageState(gDevice, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		IDirect3DDevice9_SetTextureStageState(gDevice, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	}
}

//===================================================================
// RENDER_API IMPLEMENTATION
//===================================================================
JBKRenderResult JBKRender_Create(int64_t hWindow) {
    // acquire D3D interface
    gD3D = Direct3DCreate9(D3D_SDK_VERSION);
    ASSERT(gD3D != NULL, "Failed to load DirectX9!");

    memset(&gPresentParams, 0, sizeof(D3DPRESENT_PARAMETERS));
	gPresentParams.Windowed = TRUE;
	gPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    gPresentParams.BackBufferFormat = D3DFMT_X8R8G8B8;
    gPresentParams.BackBufferWidth = 1024;
    gPresentParams.BackBufferHeight = 768;
	gPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // create D3D device
	DXCall( IDirect3D9_CreateDevice(
		gD3D,
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		(HWND)hWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&gPresentParams,
		&gDevice) );

    // set lighting off
    DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_LIGHTING, FALSE) );
	DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_ZENABLE, FALSE) );

    // create a debug font
    DXCall( D3DXCreateFont(
		gDevice, 
		18, 
		0, 
		FW_REGULAR, 
		0, 
		FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, 
		"Arial", 
		&gDebugFont) );

	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_Destroy() {
	SafeRelease(&gDevice);
	SafeRelease(&gD3D);

	return JBK_RENDER_OK;
}

JBKVertexBuffer JBKRender_CreateVertexBuffer(uint32_t sizeInBytes, JBKVertexDataType dataType) {
	IDirect3DVertexBuffer9* buffer = 0;
	int32_t stride = 0;

	DWORD fvf = DataTypeToFVF(dataType, &stride);
    DXCall( IDirect3DDevice9_CreateVertexBuffer(gDevice, sizeInBytes, 0, fvf, D3DPOOL_MANAGED, &buffer, NULL) );
	return buffer;
}

void JBKRender_DestroyVertexBuffer(JBKVertexBuffer buffer) {
	SafeRelease(&buffer);
}

void* JBKRender_LockVertexBuffer(JBKVertexBuffer buffer, uint32_t offset, uint32_t size) {
	void* data;
	DXCall( IDirect3DVertexBuffer9_Lock((IDirect3DVertexBuffer9*)buffer, offset, size, &data, 0) );
	return data;
}

void JBKRender_UnlockVertexBuffer(JBKVertexBuffer buffer) {
	DXCall( IDirect3DVertexBuffer9_Unlock((IDirect3DVertexBuffer9*)buffer) );
}

JBKNativeTextureInfo JBKRender_CreateTextureFromFile(const char* fileName) {
	JBKNativeTextureInfo result = {0};

	IDirect3DTexture9* texture;
	D3DXIMAGE_INFO info;
	DXCall( D3DXCreateTextureFromFileEx(gDevice, 
		fileName, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		&info,
		0,
		&texture) );

	result.Width = info.Width;
	result.Height = info.Height;
	result.Texture = texture;

	return result;
}

void JBKRender_SetTexture(JBKNativeTextureSampler sampler, JBKNativeTexture nativeTexture) {
	DXCall( IDirect3DDevice9_SetTexture(gDevice, sampler, nativeTexture) );
}

void JBKRender_DestroyTexture(JBKNativeTexture nativeTexture) {
	IDirect3DTexture9_Release((IDirect3DTexture9*)nativeTexture);
}

JBKRenderResult JBKRender_ClearRT(JBKRenderColor color) {
	DXCall( IDirect3DDevice9_Clear(gDevice, 0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0) );
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepth(float depth) {
	DXCall( IDirect3DDevice9_Clear(gDevice, 0, NULL, D3DCLEAR_ZBUFFER, 0, depth, 0) );
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearStencil(int32_t stencil) {
	DXCall( IDirect3DDevice9_Clear(gDevice, 0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, stencil) );
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepthStencil(float depth, int32_t stencil) {
	DXCall( IDirect3DDevice9_Clear(gDevice, 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, depth, stencil) );
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_Clear(JBKRenderColor color, float depth, int32_t stencil) {
	DXCall( IDirect3DDevice9_Clear(gDevice, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, color, depth, stencil) );
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
	IDirect3DDevice9_Present(gDevice, NULL, NULL, 0, NULL);
	return JBK_RENDER_OK;
}

void JBKRender_EnableAlphaBlending() {
	DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_ALPHABLENDENABLE, 1) );
}
void JBKRender_DisableAlphaBlending() {
	DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_ALPHABLENDENABLE, 0) );
}

void JBKRender_SetBlendAlpha8() {
	DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_BLENDOP, D3DBLENDOP_ADD) );
	DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) );
	DXCall( IDirect3DDevice9_SetRenderState(gDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) );
}

void JBKRender_DrawUserPrimitives(void* data, JBKVertexDataType datatype, JBKPrimitiveType primitivetype, uint32_t primitivecount) {
	int32_t stride = 0;
	DWORD fvf = DataTypeToFVF(datatype, &stride);
	D3DPRIMITIVETYPE d3dprimtype = PrimitiveTypeToD3D(primitivetype);

	SetupTextureStageStates(fvf);

	DXCall( IDirect3DDevice9_SetFVF(gDevice, fvf) );
	DXCall( IDirect3DDevice9_BeginScene(gDevice) );
	DXCall( IDirect3DDevice9_DrawPrimitiveUP(gDevice, d3dprimtype, primitivecount, data, stride) );
	DXCall( IDirect3DDevice9_EndScene(gDevice) );
}

void JBKRender_DrawPrimitives(JBKVertexBuffer vb, JBKPrimitiveType primitivetype, uint32_t primitivecount) {
	int32_t stride = 0;
//	DWORD fvf = DataTypeToFVF(datatype, &stride);
	D3DPRIMITIVETYPE d3dprimtype = PrimitiveTypeToD3D(primitivetype);

//	SetupTextureStageStates(datatype);

//	DXCall( IDirect3DDevice9_SetFVF(gDevice, fvf) );
	DXCall( IDirect3DDevice9_DrawPrimitive(gDevice, d3dprimtype, 0, primitivecount) );
}

void JBKRender_DrawDebugText(float x, float y, const char* text, uint32_t color) {
    RECT r;
    r.left = (LONG)x;
    r.top = (LONG)y;
    r.right = r.left;
    r.bottom = r.top;

	DXCall( IDirect3DDevice9_BeginScene(gDevice) );
	gDebugFont->lpVtbl->DrawTextA(gDebugFont, 0, text, -1, &r, DT_LEFT | DT_TOP | DT_NOCLIP, color);
	DXCall( IDirect3DDevice9_EndScene(gDevice) );
}

#endif // JBK_RENDERER_DIRECT3D9