#include "memory/heap.h"

#include <stdlib.h>

#include "memory/memorydefs.h"
#include "memory/linearallocator.h"
#include "memory/pools.h"

struct JBKHeap {
	JBKLinAlloc mAllocator;
	void* mRewindPoint;
};

JBKHeapResult JBKHeap_Create(JBKHeap** outHeap, uint32_t sizeInBytes) {
	return JBKHeap_CreateAligned(outHeap, sizeInBytes, DEFAULT_ALIGNMENT);
}

JBKHeapResult JBKHeap_CreateAligned(JBKHeap** outHeap, uint32_t sizeInBytes, uint32_t alignment) {
	// local variables
	uint8_t* mem;
	JBKLinAlloc* allocator;
	JBKHeap* heap;
	void** chainStart;

	// allocate memory
	mem = malloc(sizeInBytes);
	if (mem == NULL) {
		return JBK_HEAP_OUT_OF_MEMORY;
	}

	// place own self in this memory
	heap = (JBKHeap*)mem;
	allocator = &heap->mAllocator;

	// create the linear allocator and give it the memory after self
	if (JBKLinAlloc_InitAligned(allocator, mem + sizeof(JBKHeap), sizeInBytes - sizeof(JBKHeap), alignment) != JBK_LINALLOC_OK) {
		free(mem);
		return JBK_HEAP_ALLOCATOR_CREATION_FAILED;
	}

	// set heap members
	heap->mRewindPoint = JBKLinAlloc_GetCurrent(allocator);

	// begin the rewind chain
	chainStart = JBKLinAlloc_Allocate(allocator, sizeof(void*));
	*chainStart = NULL;

	// set the output heap
	*outHeap = heap;

	return JBK_HEAP_OK;
}

JBKHeapResult JBKHeap_Destroy(JBKHeap* heap) {
	JBKLinAllocResult allocDestructionRes = JBKLinAlloc_Cleanup(&heap->mAllocator);

	free(heap);

	if (allocDestructionRes != JBK_LINALLOC_OK) {
		return JBK_HEAP_ALLOCATOR_DESTRUCTION_FAILED;
	} 

	return JBK_HEAP_OK;
}

void* JBKHeap_Allocate(JBKHeap* heap, uint32_t size) {
	return JBKLinAlloc_Allocate(&heap->mAllocator, size);
}

void* JBKHeap_AllocateArray(JBKHeap* heap, uint32_t elemSize, uint32_t elemCount) {
	return JBKLinAlloc_Allocate(&heap->mAllocator, JBKLinAlloc_AlignedSize(&heap->mAllocator, elemSize) * elemCount);
}

JBKHeapResult JBKHeap_NewLevel(JBKHeap* heap) {
	void** mem;

	// temporary store the old rewind point
	void* oldRewind = heap->mRewindPoint;

	// update the heap rewind point
	heap->mRewindPoint = JBKLinAlloc_GetCurrent(&heap->mAllocator);

	// get some memory and store the rewind chain
	mem = JBKLinAlloc_Allocate(&heap->mAllocator, sizeof(void*));
	*mem = oldRewind;

	return JBK_HEAP_OK;
}

JBKHeapResult JBKHeap_ClearLevel(JBKHeap* heap) {
	void* prevRewind = *((void**)heap->mRewindPoint);

	JBKLinAlloc_Rewind(&heap->mAllocator, heap->mRewindPoint);

	if (prevRewind != NULL) {
		heap->mRewindPoint = prevRewind;
	} else {
		// reestablish the rewind chain head
		void** chainHead = JBKLinAlloc_Allocate(&heap->mAllocator, sizeof(void*));
		*chainHead = NULL;
	}

	return JBK_HEAP_OK;
}

JBKHeapResult JBKHeap_CreateContiguousPool(struct JBKContiguousPool** outPool, JBKHeap* heap, uint32_t elemSize, uint32_t elemCount) {
	JBKPoolResult res = JBKContiguousPool_Create(outPool, &heap->mAllocator, elemSize, elemCount);

	return res != JBK_POOL_OK ? JBK_HEAP_POOL_CREATION_FAILED : JBK_HEAP_OK;
}

JBKHeapResult JBKHeap_CreateDistributedPool(struct JBKDistributedPool** outPool, JBKHeap* heap, uint32_t elemSize) {
	JBKPoolResult res = JBKDistributedPool_Create(outPool, &heap->mAllocator, elemSize);

	return res != JBK_POOL_OK ? JBK_HEAP_POOL_CREATION_FAILED : JBK_HEAP_OK;
}
