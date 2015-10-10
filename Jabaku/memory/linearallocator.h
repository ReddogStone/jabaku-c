#ifndef JBK_LINEAR_ALLOCATOR_H
#define JBK_LINEAR_ALLOCATOR_H

#include <stdint.h>

// The linear allocator manages a linear block of memory.
// It allocates memory from this block by keeping a simple
// "next" pointer, that indicates the next free location.
// It is not able to free memory other than clearing the whole
// block in one go or rewinding the all allocation to a certain point

typedef struct JBKLinAlloc {
	uint8_t* mBegin;				// pointer to the start of the managed memory
	uint8_t* mNext;					// pointer to the free block
	uint8_t* mEnd;					// pointer to the first byte after the managed memory
	uint32_t mAlignment;			// desired alignment */
} JBKLinAlloc;

typedef enum {
	JBK_LINALLOC_OK,
	JBK_LINALLOC_OUT_OF_RANGE
} JBKLinAllocResult;

JBKLinAllocResult JBKLinAlloc_Init(JBKLinAlloc* outAllocator, void* ptr, uint32_t sizeInBytes);
JBKLinAllocResult JBKLinAlloc_InitAligned(JBKLinAlloc* outAllocator, void* ptr, uint32_t sizeInBytes, uint32_t alignment);
JBKLinAllocResult JBKLinAlloc_Cleanup(JBKLinAlloc* allocator);

uint32_t JBKLinAlloc_GetCapacity(JBKLinAlloc* allocator);
uint32_t JBKLinAlloc_GetSize(JBKLinAlloc* allocator);
void* JBKLinAlloc_GetCurrent(JBKLinAlloc* allocator);

void* JBKLinAlloc_Allocate(JBKLinAlloc* allocator, uint32_t size);
JBKLinAllocResult JBKLinAlloc_Rewind(JBKLinAlloc* allocator, void* ptr);
JBKLinAllocResult JBKLinAlloc_Clear(JBKLinAlloc* allocator);

uint32_t JBKLinAlloc_AlignedSize(JBKLinAlloc* allocator, uint32_t size);

#define LIN_INSTANCE_ALLOC(allocator, type) ((type*) JBKLinAlloc_Allocate(allocator, sizeof(type)))
#define LIN_ARRAY_ALLOC(allocator, type, size) ((type*) JBKLinAlloc_Allocate(allocator, JBKLinAlloc_AlignedSize(allocator, sizeof(type)) * size))
#define LIN_ARRAY_ALLOC_ELEMSIZE(allocator, elemSize, elemCount) (JBKLinAlloc_Allocate(allocator, JBKLinAlloc_AlignedSize(allocator, elemSize) * elemCount))

#endif // JBK_LINEAR_ALLOCATOR_H