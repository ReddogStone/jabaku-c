#ifndef JBK_D3D_UTILS_H
#define JBK_D3D_UTILS_H

#pragma warning(disable: 4005)
#include <d3d11.h>
#include "shader_input.h"
#include "primitive_type.h"

DXGI_FORMAT JBKD3DUtils_ConvertFormat(JBKShaderFormat format);
D3D11_PRIMITIVE_TOPOLOGY JBKD3DUtils_ConvertPritmitiveType(JBKPrimitiveType primitiveType);

#endif // JBK_D3D_UTILS_H
