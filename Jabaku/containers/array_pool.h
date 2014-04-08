#ifndef JBK_ARRAY_POOL_H
#define JBK_ARRAY_POOL_H

#include <stdint.h>

typedef struct JBKArrayPool {
	int8_t* m_buffer;
	int8_t* m_bufferEnd;
	int8_t* m_current;
	uint32_t m_itemSize;
} JBKArrayPool;

void JBKArrayPool_Init(JBKArrayPool* inst, void* buffer, uint32_t maxCount, uint32_t itemSize);
void JBKArrayPool_Cleanup(JBKArrayPool* inst);

uint32_t JBKArrayPool_Add(JBKArrayPool* inst, void* item);
void* JBKArrayPool_Get(JBKArrayPool* inst, uint32_t index);
void* JBKArrayPool_Remove(JBKArrayPool* inst, uint32_t index);

#endif // JBK_ARRAY_POOL_H
