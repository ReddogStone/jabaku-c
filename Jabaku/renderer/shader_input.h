#ifndef JBK_SHADER_INPUT_H
#define JBK_SHADER_INPUT_H

#include <stdint.h>

typedef enum {
	JBK_FMT_X,
	JBK_FMT_XY,
	JBK_FMT_XYZ,
	JBK_FMT_XYZW,
	JBK_FMT_COLOR,
	JBK_FMT_X_INST,
	JBK_FMT_XY_INST,
	JBK_FMT_XYZ_INST,
	JBK_FMT_XYZW_INST,
	JBK_FMT_COLOR_INST
} JBKShaderFormat;

typedef struct JBKShaderInputElement {
	const char* name;
	uint8_t index;
	JBKShaderFormat format;
	uint32_t slot;
} JBKShaderInputElement;

#endif // JBK_SHADER_INPUT_H