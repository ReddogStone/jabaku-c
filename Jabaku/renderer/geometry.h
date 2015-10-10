#ifndef JBK_GEOMETRY_H
#define JBK_GEOMETRY_H

#include <stdint.h>
#include "renderer/vertexdatatypes.h"
#include "renderer/renderapi.h"

typedef struct JBKGeometry {
	JBKVertexDataType m_dataType;
	JBKVertexBuffer m_vertexBuffer;
	JBKIndexBuffer m_indexBuffer;
} JBKGeometry;

void JBKGeometry_Create(JBKGeometry* this);

#endif // JBK_GEOMETRY_H