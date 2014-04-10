#ifdef JBK_DIRECT_3D_11

#include "renderapi.h"

#pragma warning(disable: 4005)

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

#include "utility/assertion.h"
#include "utility/dxutils.h"

#include "d3dutils.h"

static IDXGISwapChain* g_swapchain;
static ID3D11Device* g_device;
static ID3D11DeviceContext* g_devcon;
static ID3D11RenderTargetView *g_backbuffer;

struct JBKVertexBuffer {
	ID3D11Buffer* m_buffer;
};

struct JBKConstantBuffer {
	ID3D11Buffer* m_buffer;
};

struct JBKVertexShader {
	ID3D11VertexShader* m_shader;
	ID3D11InputLayout* m_inputLayout;
};

struct JBKPixelShader {
	ID3D11PixelShader* m_shader;
};

static JBKVertexBuffer g_vertexBuffers[1024];
static uint32_t g_vertexBufferCount = 0;
static JBKConstantBuffer g_constantBuffers[1024];
static uint32_t g_constantBufferCount = 0;

static JBKVertexShader g_vertexShaders[128];
static uint32_t g_vertexShaderCount = 0;
static JBKPixelShader g_pixelShaders[128];
static uint32_t g_pixelShaderCount = 0;

#define D3D_Release(x) (x)->lpVtbl->Release(x)

JBKRenderResult JBKRender_Init(void* hWindow, JBKRenderOptions *options) {
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = GetActiveWindow();
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;

	DXCall(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, 
		NULL, 0, D3D11_SDK_VERSION, &scd, &g_swapchain, &g_device, NULL, &g_devcon));

	ID3D11Resource *pBackBuffer;
	DXCall(g_swapchain->lpVtbl->GetBuffer(g_swapchain, 0, &IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer));
	DXCall(g_device->lpVtbl->CreateRenderTargetView(g_device, pBackBuffer, NULL, &g_backbuffer));
	g_devcon->lpVtbl->OMSetRenderTargets(g_devcon, 1, &g_backbuffer, 0);
	D3D_Release(pBackBuffer);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	g_devcon->lpVtbl->RSSetViewports(g_devcon, 1, &viewport);

	g_vertexBufferCount = 0;
	g_vertexShaderCount = 0;
	g_pixelShaderCount = 0;

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_Cleanup() {
	D3D_Release(g_backbuffer);
	D3D_Release(g_swapchain);
	D3D_Release(g_device);
	D3D_Release(g_devcon);

	g_vertexBufferCount = 0;
	g_vertexShaderCount = 0;
	g_pixelShaderCount = 0;

	return JBK_RENDER_OK;
}

JBKVertexBuffer* JBKRender_CreateVertexBuffer(const void* data, uint32_t sizeInBytes) {
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeInBytes;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = data;

	JBKVertexBuffer *vb = &g_vertexBuffers[g_vertexBufferCount++];
	DXCall(g_device->lpVtbl->CreateBuffer(g_device, &desc, &initData, &vb->m_buffer));
	return vb;
}
void JBKRender_DestroyVertexBuffer(JBKVertexBuffer *buffer) {
	D3D_Release(buffer->m_buffer);
}

JBKConstantBuffer* JBKRender_CreateConstantBuffer(uint32_t sizeInBytes) {
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeInBytes;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	JBKConstantBuffer *cb = &g_constantBuffers[g_constantBufferCount++];
	DXCall(g_device->lpVtbl->CreateBuffer(g_device, &desc, NULL, &cb->m_buffer));
	return cb;
}
void JBKRender_DestroyConstantBuffer(JBKConstantBuffer *buffer) {
	D3D_Release(buffer->m_buffer);
}

void* JBKRender_LockConstantBuffer(JBKConstantBuffer *buffer) {
	D3D11_MAPPED_SUBRESOURCE mapped;
	DXCall(g_devcon->lpVtbl->Map(g_devcon, (ID3D11Resource*)buffer->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	return mapped.pData;
}
void JBKRender_UnlockConstantBuffer(JBKConstantBuffer *buffer) {
	g_devcon->lpVtbl->Unmap(g_devcon, (ID3D11Resource*)buffer->m_buffer, 0);
}

static void convertInputElements(D3D11_INPUT_ELEMENT_DESC* out, JBKShaderInputElement* elements, uint32_t elementCount) {
	for (uint32_t i = 0; i < elementCount; i++) {
		out[i].SemanticName = elements[i].mName;
		out[i].SemanticIndex = 0;
		out[i].Format = JBKD3DUtils_ConvertFormat(elements[i].mFormat);
		out[i].InputSlot = i;
		out[i].AlignedByteOffset = 0;
		out[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		out[i].InstanceDataStepRate = 0;
	}
}

JBKVertexShader* JBKRender_CreateVertexShader(void* blob, int32_t sizeInBytes, JBKShaderInputElement* elements, uint32_t elementCount) {
	JBKVertexShader* shader = &g_vertexShaders[g_vertexShaderCount++];

	D3D11_INPUT_ELEMENT_DESC inputElementArray[16];
	convertInputElements(inputElementArray, elements, elementCount);
	DXCall(g_device->lpVtbl->CreateVertexShader(g_device, blob, sizeInBytes, NULL, &shader->m_shader));
	DXCall(g_device->lpVtbl->CreateInputLayout(g_device, inputElementArray, elementCount, blob, sizeInBytes, &shader->m_inputLayout));

	return shader;
}
void JBKRender_DestroyVertexShader(JBKVertexShader* shader) {
	D3D_Release(shader->m_inputLayout);
	D3D_Release(shader->m_shader);
}

JBKPixelShader* JBKRender_CreatePixelShader(void* blob, int32_t sizeInBytes) {
	JBKPixelShader* shader = &g_pixelShaders[g_pixelShaderCount++];
	DXCall(g_device->lpVtbl->CreatePixelShader(g_device, blob, sizeInBytes, NULL, &shader->m_shader));
	return shader;
}
void JBKRender_DestroyPixelShader(JBKPixelShader* shader) {
	D3D_Release(shader->m_shader);
}

JBKRenderResult JBKRender_Clear(JBKRenderColor value, float depthVal, int32_t stencilVal) {
	JBKRender_ClearRT(value);
	JBKRender_ClearDepthStencil(depthVal, stencilVal);
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_ClearRT(JBKRenderColor color) {
	float floatColor[4] = { 
		(float)((color >> 24) & 0xFF) / 255.0f,
		(float)((color >> 16) & 0xFF) / 255.0f,
		(float)((color >> 8) & 0xFF) / 255.0f,
		(float)(color & 0xFF) / 255.0f
	};
	g_devcon->lpVtbl->ClearRenderTargetView(g_devcon, g_backbuffer, floatColor);
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

JBKRenderResult JBKRender_SetEffect(JBKVertexShader* vertexShader, JBKPixelShader* pixelShader) {
	g_devcon->lpVtbl->VSSetShader(g_devcon, vertexShader->m_shader, 0, 0);
	g_devcon->lpVtbl->PSSetShader(g_devcon, pixelShader->m_shader, 0, 0);
	g_devcon->lpVtbl->IASetInputLayout(g_devcon, vertexShader->m_inputLayout);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetVertexBuffer(JBKVertexBuffer* vertexBuffer, uint32_t elementCount, uint32_t stride, uint32_t* offsets) {
	ID3D11Buffer* buffers[16];
	UINT strideArray[16];
	UINT offsetArray[16];

	for (uint32_t i = 0; (i < 16 && i < elementCount); i++) {
		buffers[i] = vertexBuffer->m_buffer;
		strideArray[i] = stride;
		offsetArray[i] = offsets[i];
	}

	g_devcon->lpVtbl->IASetVertexBuffers(g_devcon, 0, elementCount, buffers, strideArray, offsetArray);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetVSConstants(JBKConstantBuffer** constantBuffers, uint32_t bufferCount) {
	ID3D11Buffer* buffers[16];
	for (uint32_t i = 0; (i < bufferCount) && (i < 16); i++) {
		buffers[i] = constantBuffers[i]->m_buffer;
	}
	g_devcon->lpVtbl->VSSetConstantBuffers(g_devcon, 0, bufferCount, buffers);
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_SetPSConstants(JBKConstantBuffer** constantBuffers, uint32_t bufferCount) {
	ID3D11Buffer* buffers[16];
	for (uint32_t i = 0; (i < bufferCount) && (i < 16); i++) {
		buffers[i] = constantBuffers[i]->m_buffer;
	}
	g_devcon->lpVtbl->PSSetConstantBuffers(g_devcon, 0, bufferCount, buffers);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_DrawPrimitives(JBKPrimitiveType primitiveType, uint32_t vertexCount, uint32_t start) {
	g_devcon->lpVtbl->IASetPrimitiveTopology(g_devcon, JBKD3DUtils_ConvertPritmitiveType(primitiveType));
	g_devcon->lpVtbl->Draw(g_devcon, vertexCount, start);

	return JBK_RENDER_OK;
}

#endif