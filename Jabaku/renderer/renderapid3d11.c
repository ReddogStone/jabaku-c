#ifdef JBK_DIRECT_3D_11

#include "renderapi.h"

#pragma warning(disable: 4005)

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

#include "utility/assertion.h"
#include "utility/dxutils.h"

IDXGISwapChain* g_swapchain;
ID3D11Device* g_device;
ID3D11DeviceContext* devcon;
ID3D11RenderTargetView *g_backbuffer;

#define D3D_Release(x) x->lpVtbl->Release(x)

JBKRenderResult JBKRender_Init(void* hWindow, JBKRenderOptions *options) {
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = GetActiveWindow();
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;

	DXCall(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &scd, &g_swapchain, &g_device, NULL, &devcon));

	ID3D11Resource *pBackBuffer;
	DXCall(g_swapchain->lpVtbl->GetBuffer(g_swapchain, 0, &IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer));
	DXCall(g_device->lpVtbl->CreateRenderTargetView(g_device, pBackBuffer, NULL, &g_backbuffer));
	D3D_Release(pBackBuffer);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 640;
	viewport.Height = 480;

	devcon->lpVtbl->RSSetViewports(devcon, 1, &viewport);

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_Cleanup() {
	D3D_Release(g_backbuffer);
	D3D_Release(g_swapchain);
	D3D_Release(g_device);
	D3D_Release(devcon);

	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_Clear(JBKRenderColor value, float depthVal, int32_t stencilVal) {
	JBKRender_ClearRT(value);
	JBKRender_ClearDepthStencil(depthVal, stencilVal);
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearRT(JBKRenderColor color) {
	float floatColor[4] = { 
		(float)((color >> 24) & 0xFF),
		(float)((color >> 16) & 0xFF),
		(float)((color >> 8) & 0xFF),
		(float)(color & 0xFF)
	};
	devcon->lpVtbl->ClearRenderTargetView(devcon, g_backbuffer, floatColor);
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepth(float depth) {
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearStencil(int32_t stencil) {
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepthStencil(float depth, int32_t stencil) {
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearRTDef() {
	JBKRender_ClearRT(JBK_RENDER_DEF_CLEAR_COLOR);
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepthDef() {
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearStencilDef() {
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDepthStencilDef() {
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearDef() {
	JBKRender_ClearRTDef();
	JBKRender_ClearDepthStencilDef();
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_Present() {
	DXCall(g_swapchain->lpVtbl->Present(g_swapchain, 0, 0));
	return JBK_RENDER_OK;
}

#endif