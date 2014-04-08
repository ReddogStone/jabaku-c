#include "array_pool.h"

#include <string.h>
#include "utility/assertion.h"

void JBKArrayPool_Init(JBKArrayPool* inst, void* buffer, uint32_t maxCount, uint32_t itemSize) {
	inst->m_buffer = buffer;
	inst->m_bufferEnd = (int8_t*)buffer + maxCount * itemSize;
	inst->m_current = buffer;
	inst->m_itemSize = itemSize;
}
void JBKArrayPool_Cleanup(JBKArrayPool* inst) {
	inst->m_buffer = 0;
	inst->m_bufferEnd = 0;
	inst->m_current = 0;
	inst->m_itemSize = 0;
}

uint32_t JBKArrayPool_Add(JBKArrayPool* inst, void* item) {
	ASSERT(inst->m_current < inst->m_bufferEnd, "Capacity of %d items exceeded", (inst->m_bufferEnd - inst->m_buffer));

	memcpy(inst->m_current, item, inst->m_itemSize);
	uint32_t res = (inst->m_current - inst->m_buffer) / inst->m_itemSize;
	inst->m_current += inst->m_itemSize;
	return res;
}

void* JBKArrayPool_Get(JBKArrayPool* inst, uint32_t index) {
	int8_t* res = inst->m_buffer + index * inst->m_itemSize;
	ASSERT(res < inst->m_bufferEnd, "Out of bounds, at %d, capacity is %d", index, (inst->m_bufferEnd - inst->m_buffer));
	return res;
}

void* JBKArrayPool_Remove(JBKArrayPool* inst, uint32_t index) {
	int8_t* res = inst->m_buffer + index * inst->m_itemSize;
	ASSERT(res < inst->m_bufferEnd, "Out of bounds, at %d, capacity is %d", index, (inst->m_bufferEnd - inst->m_buffer));
	return res;
}
