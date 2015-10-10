#ifndef JBK_NATIVE_TEXTURE_H
#define JBK_NATIVE_TEXTURE_H

#include <stdint.h>

typedef void* JBKNativeTexture;
typedef struct JBKNativeTextureInfo {
	uint32_t Width;
	uint32_t Height;
	JBKNativeTexture Texture;
} JBKNativeTextureInfo;

typedef enum {
	JBK_SAMPLER_0,
	JBK_SAMPLER_1,
	JBK_SAMPLER_2
} JBKNativeTextureSampler;

#endif // JBK_NATIVE_TEXTURE_H