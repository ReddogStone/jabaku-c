#ifndef JBK_D3D_UTILS_H
#define JBK_D3D_UTILS_H

#pragma warning(disable: 4005)
#include <d3d11.h>
#include "shader_input.h"
#include "primitive_type.h"
#include "resource_format.h"

DXGI_FORMAT JBKD3DUtils_ConvertFormat(JBKShaderFormat format);
D3D11_PRIMITIVE_TOPOLOGY JBKD3DUtils_ConvertPritmitiveType(JBKPrimitiveType primitiveType);

DXGI_FORMAT JBKD3DUtils_ConvertResourceFormat(JBKResourceFormat format);
uint32_t JBKD3DUtils_GetFormatSize(JBKResourceFormat format);

#endif // JBK_D3D_UTILS_H
