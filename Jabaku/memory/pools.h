#ifndef JBK_POOLS_H
#define JBK_POOLS_H

#include <stdint.h>

struct JBKLinAlloc;

struct JBKContiguousPool;
typedef struct JBKContiguousPool JBKContiguousPool;

struct JBKDistributedPool;
typedef struct JBKDistributedPool JBKDistributedPool;

typedef enum {
	JBK_POOL_OK,
	JBK_POOL_NULL_FREE
} JBKPoolResult;

JBKPoolResult JBKContiguousPool_Create(JBKContiguousPool** outPool, struct JBKLinAlloc* allocator, uint32_t elemSize, uint32_t elemCount);
void* JBKContiguousPool_Alloc(JBKContiguousPool* pool);
JBKPoolResult JBKContiguousPool_Free(JBKContiguousPool* pool, void* element);

JBKPoolResult JBKDistributedPool_Create(JBKDistributedPool** outPool, struct JBKLinAlloc* allocator, uint32_t elemSize);
void* JBKDistributedPool_Alloc(JBKDistributedPool* pool);
JBKPoolResult JBKDistributedPool_Free(JBKDistributedPool* pool, void* element);

#endif // JBK_POOLS_H