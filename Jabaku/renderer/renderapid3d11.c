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

struct JBKNativeTexture {
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* shaderView;
};

static JBKVertexBuffer g_vertexBuffers[1024];
static uint32_t g_vertexBufferCount = 0;
static JBKConstantBuffer g_constantBuffers[1024];
static uint32_t g_constantBufferCount = 0;

static JBKVertexShader g_vertexShaders[128];
static uint32_t g_vertexShaderCount = 0;
static JBKPixelShader g_pixelShaders[128];
static uint32_t g_pixelShaderCount = 0;

static JBKNativeTexture g_textures[2048];
static uint32_t g_textureCount = 0;

static ID3D11BlendState* g_blendStateNoBlend = NULL;
static ID3D11BlendState* g_blendStateAlpha = NULL;

static ID3D11SamplerState* g_defaultSamplerState = NULL;

#define D3D_Release(x) (x)->lpVtbl->Release(x)

JBKRenderResult JBKRender_Init(void* hWindow, JBKRenderOptions *options) {
	DXGI_SWAP_CHAIN_DESC scd = {0};

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
	viewport.Width = options ? options->sx : 800.0f;
	viewport.Height = options ? options->sy : 600.0f;

	g_devcon->lpVtbl->RSSetViewports(g_devcon, 1, &viewport);

	g_vertexBufferCount = 0;
	g_vertexShaderCount = 0;
	g_pixelShaderCount = 0;

	D3D11_BLEND_DESC blendStateNoBlend = { 0 };
	blendStateNoBlend.RenderTarget[0].BlendEnable = FALSE;
	blendStateNoBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DXCall(g_device->lpVtbl->CreateBlendState(g_device, &blendStateNoBlend, &g_blendStateNoBlend));

	D3D11_BLEND_DESC blendStateAlpha = { 0 };
	blendStateAlpha.RenderTarget[0].BlendEnable = TRUE;
	blendStateAlpha.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateAlpha.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateAlpha.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateAlpha.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateAlpha.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateAlpha.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateAlpha.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
	DXCall(g_device->lpVtbl->CreateBlendState(g_device, &blendStateAlpha, &g_blendStateAlpha));

	D3D11_SAMPLER_DESC samplerDesc = { 0 };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DXCall(g_device->lpVtbl->CreateSamplerState(g_device, &samplerDesc, &g_defaultSamplerState));

	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_Cleanup() {
	D3D_Release(g_backbuffer);
	D3D_Release(g_swapchain);
	D3D_Release(g_device);
	D3D_Release(g_devcon);

	D3D_Release(g_blendStateNoBlend);
	D3D_Release(g_blendStateAlpha);

	D3D_Release(g_defaultSamplerState);

	g_vertexBufferCount = 0;
	g_constantBufferCount = 0;
	g_vertexShaderCount = 0;
	g_pixelShaderCount = 0;
	g_textureCount = 0;

	return JBK_RENDER_OK;
}

JBKVertexBuffer* createVertexBuffer(uint32_t sizeInBytes, int8_t dynamic, void* data) {
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeInBytes;
	desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (dynamic) {
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = data;

	JBKVertexBuffer *vb = &g_vertexBuffers[g_vertexBufferCount++];
	DXCall(g_device->lpVtbl->CreateBuffer(g_device, &desc, data ? &initData : NULL, &vb->m_buffer));
	return vb;
}

JBKVertexBuffer* JBKRender_CreateVertexBuffer(void* data, uint32_t sizeInBytes) {
	return createVertexBuffer(sizeInBytes, 0, data);
}
JBKVertexBuffer* JBKRender_CreateDynamicVertexBuffer(uint32_t sizeInBytes) {
	return createVertexBuffer(sizeInBytes, 1, NULL);
}
void JBKRender_DestroyVertexBuffer(JBKVertexBuffer *buffer) {
	D3D_Release(buffer->m_buffer);
	buffer->m_buffer = NULL;
}
void* JBKRender_LockVertexBuffer(JBKVertexBuffer *buffer) {
	D3D11_MAPPED_SUBRESOURCE mapped;
	DXCall(g_devcon->lpVtbl->Map(g_devcon, (ID3D11Resource*)buffer->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	return mapped.pData;
}
void JBKRender_UnlockVertexBuffer(JBKVertexBuffer *buffer) {
	g_devcon->lpVtbl->Unmap(g_devcon, (ID3D11Resource*)buffer->m_buffer, 0);
}

JBKConstantBuffer* JBKRender_CreateConstantBuffer(uint32_t sizeInBytes) {
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeInBytes;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	JBKConstantBuffer *cb = &g_constantBuffers[g_constantBufferCount++];
	DXCall(g_device->lpVtbl->CreateBuffer(g_device, &desc, NULL, &cb->m_buffer));
	return cb;
}
void JBKRender_DestroyConstantBuffer(JBKConstantBuffer *buffer) {
	D3D_Release(buffer->m_buffer);
	buffer->m_buffer = NULL;
}

void* JBKRender_LockConstantBuffer(JBKConstantBuffer *buffer) {
	D3D11_MAPPED_SUBRESOURCE mapped;
	DXCall(g_devcon->lpVtbl->Map(g_devcon, (ID3D11Resource*)buffer->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	return mapped.pData;
}
void JBKRender_UnlockConstantBuffer(JBKConstantBuffer *buffer) {
	g_devcon->lpVtbl->Unmap(g_devcon, (ID3D11Resource*)buffer->m_buffer, 0);
}

uint32_t getElementSize(JBKShaderFormat format) {
	switch (format) {
	case JBK_FMT_X:
	case JBK_FMT_X_INST:
		return sizeof(float);
	case JBK_FMT_XY:
	case JBK_FMT_XY_INST:
		return 2 * sizeof(float);
	case JBK_FMT_XYZ:
	case JBK_FMT_XYZ_INST:
		return 3 * sizeof(float);
	case JBK_FMT_XYZW:
	case JBK_FMT_XYZW_INST:
		return 4 * sizeof(float);
	case JBK_FMT_COLOR:
	case JBK_FMT_COLOR_INST:
		return sizeof(uint32_t);
	}
	return 0;
}

uint8_t isInstanced(JBKShaderFormat format) {
	switch (format) {
	case JBK_FMT_X:
	case JBK_FMT_XY:
	case JBK_FMT_XYZ:
	case JBK_FMT_XYZW:
	case JBK_FMT_COLOR:
		return 0;
	case JBK_FMT_X_INST:
	case JBK_FMT_XY_INST:
	case JBK_FMT_XYZ_INST:
	case JBK_FMT_XYZW_INST:
	case JBK_FMT_COLOR_INST:
		return 1;
	}
	return 0;
}

static void convertInputElements(D3D11_INPUT_ELEMENT_DESC* out, JBKShaderInputElement* elements, uint32_t elementCount) {
	uint32_t offsets[16] = { 0 };

	for (uint32_t i = 0; i < elementCount; i++) {
		uint32_t slot = elements[i].slot;
		JBKShaderFormat format = elements[i].format;
		uint8_t instanced = isInstanced(format);

		out[i].SemanticName = elements[i].name;
		out[i].SemanticIndex = elements[i].index;
		out[i].Format = JBKD3DUtils_ConvertFormat(format);
		out[i].InputSlot = slot;
		out[i].AlignedByteOffset = offsets[slot];
		out[i].InputSlotClass = instanced ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
		out[i].InstanceDataStepRate = instanced ? 1 : 0;

		offsets[slot] += getElementSize(format);
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

	shader->m_inputLayout = NULL;
	shader->m_shader = NULL;
}

JBKPixelShader* JBKRender_CreatePixelShader(void* blob, int32_t sizeInBytes) {
	JBKPixelShader* shader = &g_pixelShaders[g_pixelShaderCount++];
	DXCall(g_device->lpVtbl->CreatePixelShader(g_device, blob, sizeInBytes, NULL, &shader->m_shader));
	return shader;
}
void JBKRender_DestroyPixelShader(JBKPixelShader* shader) {
	D3D_Release(shader->m_shader);
	shader->m_shader = NULL;
}

JBKNativeTexture* JBKRender_CreateTexture(uint32_t width, uint32_t height, JBKResourceFormat format, void* data) {
	JBKNativeTexture* result = &g_textures[g_textureCount++];

	// Create texture
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = JBKD3DUtils_ConvertResourceFormat(format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	desc.CPUAccessFlags = 0;
//	desc.MiscFlags = 0;

	uint32_t formatSize = JBKD3DUtils_GetFormatSize(format);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	initData.SysMemPitch = formatSize * width;
	initData.SysMemSlicePitch = formatSize * width * height;

	DXCall(g_device->lpVtbl->CreateTexture2D(g_device, &desc, &initData, &result->texture));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = { 0 };
	shaderResourceViewDesc.Format = format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	HRESULT hr = g_device->lpVtbl->CreateShaderResourceView(g_device, (ID3D11Resource*)result->texture, &shaderResourceViewDesc, &result->shaderView);
	if (FAILED(hr)) {
		D3D_Release(result->texture);
		DXCall(hr);
	}

	return result;
}
void JBKRender_DestroyTexture(JBKNativeTexture* texture) {
	D3D_Release(texture->shaderView);
	D3D_Release(texture->texture);
	texture->shaderView = NULL;
	texture->texture = NULL;
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
	HRESULT presentResult = g_swapchain->lpVtbl->Present(g_swapchain, 0, 0);
	switch (presentResult) {
	case DXGI_STATUS_OCCLUDED:
		return JBK_RENDER_OK;
	default:
		ASSERT(presentResult == S_OK, "Present failed: %x", presentResult);
	}
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetEffect(const JBKVertexShader* vertexShader, const JBKPixelShader* pixelShader) {
	g_devcon->lpVtbl->VSSetShader(g_devcon, vertexShader->m_shader, 0, 0);
	g_devcon->lpVtbl->PSSetShader(g_devcon, pixelShader->m_shader, 0, 0);
	g_devcon->lpVtbl->IASetInputLayout(g_devcon, vertexShader->m_inputLayout);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetVertexBuffers(JBKVertexBuffer* const* vertexBuffers, uint32_t count, const uint32_t* strides, const uint32_t* offsets) {
	ID3D11Buffer* buffers[16];

	for (uint32_t i = 0; (i < 16 && i < count); i++) {
		buffers[i] = vertexBuffers[i]->m_buffer;
	}

	g_devcon->lpVtbl->IASetVertexBuffers(g_devcon, 0, count, buffers, strides, offsets);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetVSConstants(JBKConstantBuffer* const* constantBuffers, uint32_t bufferCount) {
	ASSERT(bufferCount < 16, "Too many buffers (%d)", bufferCount);

	ID3D11Buffer* buffers[16];
	for (uint32_t i = 0; (i < bufferCount) && (i < 16); i++) {
		buffers[i] = constantBuffers[i]->m_buffer;
	}
	g_devcon->lpVtbl->VSSetConstantBuffers(g_devcon, 0, bufferCount, buffers);
	return JBK_RENDER_OK;
}
JBKRenderResult JBKRender_SetPSConstants(JBKConstantBuffer* const* constantBuffers, uint32_t bufferCount) {
	ASSERT(bufferCount <= 16, "Too many buffers (%d)", bufferCount);

	ID3D11Buffer* buffers[16];
	for (uint32_t i = 0; i < bufferCount; i++) {
		buffers[i] = constantBuffers[i]->m_buffer;
	}
	g_devcon->lpVtbl->PSSetConstantBuffers(g_devcon, 0, bufferCount, buffers);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetTextures(JBKNativeTexture* const* textures, uint32_t count) {
	ASSERT(count <= 16, "Too many textures (%d)", count);

	ID3D11ShaderResourceView* views[16];
	for (uint32_t i = 0; i < count; i++) {
		views[i] = textures[i]->shaderView;
	}
	g_devcon->lpVtbl->PSSetShaderResources(g_devcon, 0, count, views);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetDefaultSampling(uint32_t textureCount) {
	ASSERT(textureCount <= 16, "Too many textures (%d)", textureCount);

	ID3D11SamplerState* states[16];
	for (uint32_t i = 0; i < textureCount; i++) {
		states[i] = g_defaultSamplerState;
	}
	g_devcon->lpVtbl->PSSetSamplers(g_devcon, 0, textureCount, states);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_SetBlendAlpha8() {
	g_devcon->lpVtbl->OMSetBlendState(g_devcon, g_blendStateAlpha, 0, 0xFFFFFFFF);
	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_DrawPrimitives(JBKPrimitiveType primitiveType, uint32_t vertexCount, uint32_t start) {
	g_devcon->lpVtbl->IASetPrimitiveTopology(g_devcon, JBKD3DUtils_ConvertPritmitiveType(primitiveType));
	g_devcon->lpVtbl->Draw(g_devcon, vertexCount, start);

	return JBK_RENDER_OK;
}

JBKRenderResult JBKRender_DrawInstancedPrimitives(JBKPrimitiveType primitiveType, uint32_t vertexCount, uint32_t start, uint32_t instanceCount) {
	g_devcon->lpVtbl->IASetPrimitiveTopology(g_devcon, JBKD3DUtils_ConvertPritmitiveType(primitiveType));
	g_devcon->lpVtbl->DrawInstanced(g_devcon, vertexCount, instanceCount, start, 0);

	return JBK_RENDER_OK;
}

#endif