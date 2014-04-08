#ifndef JBK_ASSERION_H
#define JBK_ASSERION_H

#define ASSERT(condition, format, ...) \
	JBKUtility_Assert(condition, __FILE__, __LINE__, format, __VA_ARGS__);

void JBKUtility_Assert(int condition, const char *file, unsigned line, const char * format, ...);
void JBKUtility_SetUnitTestMode(int active);

#endif JBK_ASSERION_H