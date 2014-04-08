#ifndef JBK_SHADER_INPUT_H
#define JBK_SHADER_INPUT_H

typedef enum {
	JBK_FMT_X,
	JBK_FMT_XY,
	JBK_FMT_XYZ,
	JBK_FMT_XYZW,
	JBK_FMT_COLOR
} JBKShaderFormat;

typedef struct JBKShaderInputElement {
	const char* mName;
	JBKShaderFormat mFormat;
} JBKShaderInputElement;

#endif // JBK_SHADER_INPUT_H