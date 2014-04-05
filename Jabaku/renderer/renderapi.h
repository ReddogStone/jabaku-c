#ifndef JBK_RENDER_API_H
#define JBK_RENDER_API_H

#include <stdint.h>
#include "renderer/vertexdatatypes.h"
#include "renderer/nativetexture.h"

// typedefs
typedef enum {
	JBK_RENDER_OK,
	JBK_RENDER_ERROR
} JBKRenderResult;

typedef enum {
	JBK_PRIMITIVE_POINT_LIST,
	JBK_PRIMITIVE_LINE_LIST,
	JBK_PRIMITIVE_LINE_STRIP,
	JBK_PRIMITIVE_TRIANGLE_LIST,
	JBK_PRIMITIVE_TRIANGLE_STRIP
} JBKPrimitiveType;

typedef struct JBKRenderOptions {
	int dummy;
} JBKRenderOptions;

typedef uint32_t JBKRenderColor;

typedef void* JBKVertexBuffer;
typedef void* JBKIndexBuffer;

// constants
#ifdef _DEBUG
#define JBK_RENDER_DEF_CLEAR_COLOR 0xFFFF00FF
#else
#define JBK_RENDER_DEF_CLEAR_COLOR 0xFF000000
#endif

// API
JBKRenderResult JBKRender_Init(void* hWindow, JBKRenderOptions *options);
JBKRenderResult JBKRender_Cleanup();

JBKVertexBuffer JBKRender_CreateVertexBuffer(uint32_t sizeInBytes, JBKVertexDataType dataType);
void JBKRender_DestroyVertexBuffer(JBKVertexBuffer buffer);
void* JBKRender_LockVertexBuffer(JBKVertexBuffer buffer, uint32_t offset, uint32_t size);
void JBKRender_UnlockVertexBuffer(JBKVertexBuffer buffer);

JBKNativeTextureInfo JBKRender_CreateTextureFromFile(const char* fileName);
void JBKRender_SetTexture(JBKNativeTextureSampler sampler, JBKNativeTexture nativeTexture);
void JBKRender_DestroyTexture(JBKNativeTexture nativeTexture);

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

JBKRenderResult JBKRender_EnableTexture();
JBKRenderResult JBKRender_EnableAlphaBlending();
JBKRenderResult JBKRender_DisableAlphaBlending();

JBKRenderResult JBKRender_SetBlendAlpha8();

JBKRenderResult JBKRender_DrawUserPrimitives(void* data, JBKVertexDataType datatype, JBKPrimitiveType primitivetype, uint32_t primitivecount);
JBKRenderResult JBKRender_DrawPrimitives(JBKVertexBuffer vb, JBKPrimitiveType primitivetype, uint32_t primitivecount);

JBKRenderResult JBKRender_DrawDebugText(float x, float y, const char* text, uint32_t color);

#endif // JBK_RENDER_API_H