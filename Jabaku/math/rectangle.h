#ifndef JBK_RECTANGLE_H
#define JBK_RECTANGLE_H

typedef struct JBKRectangle {
	float x;
	float y;
	float width;
	float height;
} JBKRectangle;

void JBKRectangle_InitWithSize(JBKRectangle* inst, float x, float y, float width, float height);
void JBKRectangle_InitWithExtents(JBKRectangle* inst, float left, float top, float right, float bottom);
void JBKRectangle_InitWithCenterSize(JBKRectangle* inst, float centerX, float centerY, float width, float height);

#endif // JBK_RECTANGLE_H