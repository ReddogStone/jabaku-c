#ifndef JBK_TEXTURE_H
#define JBK_TEXTURE_H

#include <stdint.h>

#include "renderer/nativetexture.h"

typedef struct JBKTexture JBKTexture;

uint32_t JBKTexture_GetSize();
JBKTexture* JBKTexture_Create(void* memory, JBKNativeTexture nativeTexture);

#endif // JBK_TEXTURE_H