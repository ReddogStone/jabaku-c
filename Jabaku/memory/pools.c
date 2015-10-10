#include "memory/pools.h"

#include "memory/linearallocator.h"
#include "memory/memorydefs.h"

struct JBKContiguousPool {
	void* mFreeChain;
};

JBKPoolResult JBKContiguousPool_Create(JBKContiguousPool** outPool, JBKLinAlloc* allocator, uint32_t elemSize, uint32_t elemCount) {
	// local vars
	uint32_t i;
	JBKContiguousPool *pool;
	void** elements;
	void* lastElement;

	// allocate enough memory for the pool struct and the pool elements
	elemSize = elemSize < sizeof(void*) ? sizeof(void*) : elemSize;
	pool = LIN_INSTANCE_ALLOC(allocator, JBKContiguousPool);
	elements = LIN_ARRAY_ALLOC_ELEMSIZE(allocator, elemSize, elemCount);

	// chain up the free elements
	lastElement = NULL;
	for (i = 0; i < elemCount; i++) {
		elements[i] = lastElement;
		lastElement = &elements[i];
	}
	pool->mFreeChain = lastElement;

	// set the resulting pool
	*outPool = pool;

	return JBK_POOL_OK;
}

void* JBKContiguousPool_Alloc(JBKContiguousPool* pool) {
	void* res = pool->mFreeChain;

	if (res) {
 		pool->mFreeChain = *((void**)pool->mFreeChain);
	}

	return res;
}

JBKPoolResult JBKContiguousPool_Free(JBKContiguousPool* pool, void* element) {
	if (element == NULL) {
		return JBK_POOL_NULL_FREE;
	}

	*((void**)element) = pool->mFreeChain;

	pool->mFreeChain = element;

	return JBK_POOL_OK;
}

struct JBKDistributedPool {
	uint32_t mElemSize;
	JBKLinAlloc* mAllocator;
	void* mFreeChain;
};

JBKPoolResult JBKDistributedPool_Create(JBKDistributedPool** outPool, struct JBKLinAlloc* allocator, uint32_t elemSize) {
	// local vars
	JBKDistributedPool *pool;

	// allocate memory and init
	pool = LIN_INSTANCE_ALLOC(allocator, JBKDistributedPool);
	pool->mElemSize = elemSize;
	pool->mAllocator = allocator;

	// start empty free chain
	pool->mFreeChain = NULL;

	// set the resulting pool
	*outPool = pool;

	return JBK_POOL_OK;
}

void* JBKDistributedPool_Alloc(JBKDistributedPool* pool) {
	void* res = pool->mFreeChain;

	// if the free chain is empty allocate a new one
	if (res == NULL) {
		res = JBKLinAlloc_Allocate(pool->mAllocator, pool->mElemSize);
	} else {
		// the free chain wasn't empty so advance the free chain head
		pool->mFreeChain = *((void**)pool->mFreeChain);
	}

	return res;
}

JBKPoolResult JBKDistributedPool_Free(JBKDistributedPool* pool, void* element) {
	if (element == NULL) {
		return JBK_POOL_NULL_FREE;
	}

	*((void**)element) = pool->mFreeChain;

	pool->mFreeChain = element;

	return JBK_POOL_OK;
}
