#include "memory/memory.h"

#include <stdlib.h>

#include "memory/heap.h"
#include "utility/assertion.h"

static JBKHeap* gHeaps[JBK_MEM_ENUM_SIZE];

void JBKMem_Init(uint32_t* sizes, uint32_t count) {
	uint32_t i;

	for (i = 0; i < JBK_MEM_ENUM_SIZE; i++) {
		uint32_t size = 1024 * 1024; // some default stack size
		if (i < count) {
			size = sizes[i];
		}
		JBKHeap_Create(&gHeaps[i], size);
	}
}

void JBKMem_Cleanup() {
	uint32_t i;
	for (i = 0; i < JBK_MEM_ENUM_SIZE; i++) {
		JBKHeap_Destroy(gHeaps[i]);
	}
}

void* JBKMem_Alloc(JBKMemType memType, uint32_t size) {
	JBKHeap* heap = gHeaps[memType];
	return JBKHeap_Allocate(heap, size);
}

void* JBKMem_AllocArray(JBKMemType memType, uint32_t elemSize, uint32_t elemCount) {
	JBKHeap* heap = gHeaps[memType];
	return JBKHeap_AllocateArray(heap, elemSize, elemCount);
}

void JBKMem_NewLevel(JBKMemType memType) {
	JBKHeap* heap = gHeaps[memType];
	ASSERT(JBKHeap_NewLevel(heap) == JBK_HEAP_OK, "Failed to create a new heap level for the mem type (%d)", memType);
}

void JBKMem_ClearLevel(JBKMemType memType) {
	JBKHeap* heap = gHeaps[memType];
	ASSERT(JBKHeap_ClearLevel(heap) == JBK_HEAP_OK, "Failed to clear a heap level for the mem type (%d)", memType);
}

struct JBKContiguousPool* JBKMem_CreateContiguousPool(JBKMemType memType, uint32_t elemSize, uint32_t elemCount) {
	struct JBKContiguousPool *res;
	JBKHeap* heap = gHeaps[memType];
	ASSERT(JBKHeap_CreateContiguousPool(&res, heap, elemSize, elemCount) == JBK_HEAP_OK, "Failed to create a contiguous pool for the mem type (%d)", memType);

	return res;
}

struct JBKDistributedPool* JBKMem_CreateDistributedPool(JBKMemType memType, uint32_t elemSize) {
	struct JBKDistributedPool *res;
	JBKHeap* heap = gHeaps[memType];
	ASSERT(JBKHeap_CreateDistributedPool(&res, heap, elemSize) == JBK_HEAP_OK, "Failed to create a distributed pool for the mem type (%d)", memType);

	return res;
}