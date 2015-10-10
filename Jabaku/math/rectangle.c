#include "rectangle.h"

void JBKRectangle_InitWithSize(JBKRectangle* inst, float x, float y, float width, float height) {
	inst->x = x;
	inst->y = y;
	inst->width = width;
	inst->height = height;
}

void JBKRectangle_InitWithExtents(JBKRectangle* inst, float left, float top, float right, float bottom) {
	inst->x = left;
	inst->y = top;
	inst->width = right - left;
	inst->height = bottom - top;
}

void JBKRectangle_InitWithCenterSize(JBKRectangle* inst, float centerX, float centerY, float width, float height) {
	inst->x = centerX - 0.5f * width;
	inst->y = centerY - 0.5f * width;
	inst->width = width;
	inst->height = height;
}