#ifndef JBK_RENDER_API_H
#define JBK_RENDER_API_H

#include <stdint.h>
#include "renderer/nativetexture.h"

#include "shader_input.h"
#include "primitive_type.h"

// typedefs
typedef enum {
	JBK_RENDER_OK,
	JBK_RENDER_ERROR
} JBKRenderResult;

typedef struct JBKRenderOptions {
	int dummy;
} JBKRenderOptions;

typedef uint32_t JBKRenderColor;

typedef struct JBKVertexBuffer JBKVertexBuffer;
typedef struct JBKIndexBuffer JBKIndexBuffer;

typedef struct JBKVertexShader JBKVertexShader;
typedef struct JBKPixelShader JBKPixelShader;

// constants
#ifdef _DEBUG
#define JBK_RENDER_DEF_CLEAR_COLOR 0xFFFF00FF
#else
#define JBK_RENDER_DEF_CLEAR_COLOR 0xFF000000
#endif

// API
JBKRenderResult JBKRender_Init(void* hWindow, JBKRenderOptions *options);
JBKRenderResult JBKRender_Cleanup();

JBKVertexBuffer* JBKRender_CreateVertexBuffer(const void *data, uint32_t sizeInBytes);
void JBKRender_DestroyVertexBuffer(JBKVertexBuffer *buffer);

JBKNativeTextureInfo JBKRender_CreateTextureFromFile(const char* fileName);
void JBKRender_SetTexture(JBKNativeTextureSampler sampler, JBKNativeTexture nativeTexture);
void JBKRender_DestroyTexture(JBKNativeTexture nativeTexture);

JBKVertexShader* JBKRender_CreateVertexShader(void* blob, int32_t sizeInBytes, JBKShaderInputElement* elements, uint32_t elementCount);
void JBKRender_DestroyVertexShader(JBKVertexShader* shader);
JBKPixelShader* JBKRender_CreatePixelShader(void* blob, int32_t sizeInBytes);
void JBKRender_DestroyPixelShader(JBKPixelShader* shader);

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

JBKRenderResult JBKRender_SetEffect(JBKVertexShader* vertexShader, JBKPixelShader* pixelShader);
JBKRenderResult JBKRender_SetVertexBuffer(JBKVertexBuffer* vertexBuffer, uint32_t elementCount, uint32_t stride, uint32_t* offsets);

JBKRenderResult JBKRender_SetBlendAlpha8();

JBKRenderResult JBKRender_DrawPrimitives(JBKPrimitiveType primitiveType, uint32_t vertexCount, uint32_t start);

#endif // JBK_RENDER_API_H