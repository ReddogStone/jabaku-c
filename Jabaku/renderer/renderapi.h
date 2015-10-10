#ifndef JBK_RENDER_API_H
#define JBK_RENDER_API_H

#include <stdint.h>

#include "shader_input.h"
#include "primitive_type.h"
#include "resource_format.h"

// typedefs
typedef enum {
	JBK_RENDER_OK,
	JBK_RENDER_ERROR
} JBKRenderResult;

typedef struct JBKRenderOptions {
	uint32_t sx;
	uint32_t sy;
} JBKRenderOptions;

typedef uint32_t JBKRenderColor;

typedef struct JBKVertexBuffer JBKVertexBuffer;
typedef struct JBKIndexBuffer JBKIndexBuffer;
typedef struct JBKConstantBuffer JBKConstantBuffer;

typedef struct JBKVertexShader JBKVertexShader;
typedef struct JBKPixelShader JBKPixelShader;

typedef struct JBKNativeTexture JBKNativeTexture;

// constants
#ifdef _DEBUG
#define JBK_RENDER_DEF_CLEAR_COLOR 0xFFFF00FF
#else
#define JBK_RENDER_DEF_CLEAR_COLOR 0xFF000000
#endif

// API
JBKRenderResult JBKRender_Init(void* hWindow, JBKRenderOptions *options);
JBKRenderResult JBKRender_Cleanup();

JBKVertexBuffer* JBKRender_CreateVertexBuffer(void* data, uint32_t sizeInBytes);
JBKVertexBuffer* JBKRender_CreateDynamicVertexBuffer(uint32_t sizeInBytes);
void JBKRender_DestroyVertexBuffer(JBKVertexBuffer *buffer);
void* JBKRender_LockVertexBuffer(JBKVertexBuffer *buffer);
void JBKRender_UnlockVertexBuffer(JBKVertexBuffer *buffer);

JBKConstantBuffer* JBKRender_CreateConstantBuffer(uint32_t sizeInBytes);
void JBKRender_DestroyConstantBuffer(JBKConstantBuffer *buffer);
void* JBKRender_LockConstantBuffer(JBKConstantBuffer *buffer);
void JBKRender_UnlockConstantBuffer(JBKConstantBuffer *buffer);

JBKVertexShader* JBKRender_CreateVertexShader(void* blob, int32_t sizeInBytes, JBKShaderInputElement* elements, uint32_t elementCount);
void JBKRender_DestroyVertexShader(JBKVertexShader* shader);
JBKPixelShader* JBKRender_CreatePixelShader(void* blob, int32_t sizeInBytes);
void JBKRender_DestroyPixelShader(JBKPixelShader* shader);

JBKNativeTexture* JBKRender_CreateTexture(uint32_t width, uint32_t height, JBKResourceFormat format, void* data);
void JBKRender_DestroyTexture(JBKNativeTexture* texture);

JBKRenderResult JBKRender_Clear(JBKRenderColor value, float depthVal, int32_t stencilVal);
JBKRenderResult JBKRender_ClearRT(JBKRenderColor color);
JBKRenderResult JBKRender_ClearDepth(float depth);
JBKRenderResult JBKRender_ClearStencil(int32_t stencil);
JBKRenderResult JBKRender_ClearDepthStencil(float depth, int32_t stencil);
JBKRenderResult JBKRender_ClearRTDef();
JBKRenderResult JBKRender_ClearDepthDef();
JBKRenderResult JBKRender_ClearStencilDef();
JBKRenderResult JBKRender_ClearDepthStencilDef();
JBKRenderResult JBKRender_ClearDef();

JBKRenderResult JBKRender_Present();

JBKRenderResult JBKRender_SetEffect(const JBKVertexShader* vertexShader, const JBKPixelShader* pixelShader);
JBKRenderResult JBKRender_SetVertexBuffers(JBKVertexBuffer* const* vertexBuffers, uint32_t count, const uint32_t* strides, const uint32_t* offsets);
JBKRenderResult JBKRender_SetVSConstants(JBKConstantBuffer* const* constantBuffers, uint32_t bufferCount);
JBKRenderResult JBKRender_SetPSConstants(JBKConstantBuffer* const* constantBuffers, uint32_t bufferCount);

JBKRenderResult JBKRender_SetTextures(JBKNativeTexture* const* textures, uint32_t count);
JBKRenderResult JBKRender_SetDefaultSampling(uint32_t textureCount);

JBKRenderResult JBKRender_SetBlendAlpha8();

JBKRenderResult JBKRender_DrawPrimitives(JBKPrimitiveType primitiveType, uint32_t vertexCount, uint32_t start);
JBKRenderResult JBKRender_DrawInstancedPrimitives(JBKPrimitiveType primitiveType, uint32_t vertexCount, uint32_t start, uint32_t instanceCount);

#endif // JBK_RENDER_API_H