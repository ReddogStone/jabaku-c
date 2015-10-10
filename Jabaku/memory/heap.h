#ifndef JBK_HEAP_H
#define JBK_HEAP_H

#include <stdint.h>

struct JBKContiguousPool;
struct JBKDistributedPool;

struct JBKHeap;
typedef struct JBKHeap JBKHeap;

typedef enum {
	JBK_HEAP_OK,
	JBK_HEAP_OUT_OF_MEMORY,
	JBK_HEAP_ALLOCATOR_CREATION_FAILED,
	JBK_HEAP_ALLOCATOR_DESTRUCTION_FAILED,
	JBK_HEAP_POOL_CREATION_FAILED
} JBKHeapResult;

JBKHeapResult JBKHeap_Create(JBKHeap** outHeap, uint32_t sizeInBytes);
JBKHeapResult JBKHeap_CreateAligned(JBKHeap** outHeap, uint32_t sizeInBytes, uint32_t alignment);
JBKHeapResult JBKHeap_Destroy(JBKHeap* heap);

void* JBKHeap_Allocate(JBKHeap* heap, uint32_t size);
void* JBKHeap_AllocateArray(JBKHeap* heap, uint32_t elemSize, uint32_t elemCount);
JBKHeapResult JBKHeap_NewLevel(JBKHeap* heap);
JBKHeapResult JBKHeap_ClearLevel(JBKHeap* heap);

JBKHeapResult JBKHeap_CreateContiguousPool(struct JBKContiguousPool** outPool, JBKHeap* heap, uint32_t elemSize, uint32_t elemCount);
JBKHeapResult JBKHeap_CreateDistributedPool(struct JBKDistributedPool** outPool, JBKHeap* heap, uint32_t elemSize);

#endif // JBK_HEAP_H