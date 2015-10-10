#include "memory/linearallocator.h"

#include "memory/memorydefs.h"
#include "utility/assertion.h"

static const uint32_t FREED_MEMORY_MAGIC_NUMBER = 0xBAADBEEF;
static const uint32_t NONINIT_MEMORY_MAGIC_NUMBER = 0xF00DBAAD;

static uint32_t AlignedSize(uint32_t size, uint32_t alignment) { 
	return (size + (alignment - 1)) & ~(alignment - 1);
}

static uint8_t* AlignedPointer(uint8_t* ptr, uint32_t alignment) { 
	return ptr + ((alignment - ((uint32_t)ptr & (alignment - 1))) & (alignment - 1));
}

JBKLinAllocResult JBKLinAlloc_InitAligned(JBKLinAlloc* outAllocator, void* ptr, uint32_t sizeInBytes, uint32_t alignment) {
	// setup the allocator
	outAllocator->mBegin = AlignedPointer(ptr, alignment);
	outAllocator->mNext = outAllocator->mBegin;
	outAllocator->mEnd = (uint8_t*)(((uint32_t)ptr + sizeInBytes) & ~(alignment - 1));
	outAllocator->mAlignment = alignment;

	return JBK_LINALLOC_OK;
}

JBKLinAllocResult JBKLinAlloc_Init(JBKLinAlloc* outAllocator, void* ptr, uint32_t sizeInBytes) {
	return JBKLinAlloc_InitAligned(outAllocator, ptr, sizeInBytes, DEFAULT_ALIGNMENT);
}

JBKLinAllocResult JBKLinAlloc_Cleanup(JBKLinAlloc* allocator) {
	// clear the managed memory
	JBKLinAlloc_Clear(allocator);

	return JBK_LINALLOC_OK;
}

uint32_t JBKLinAlloc_GetCapacity(JBKLinAlloc* allocator) { 
	return allocator->mEnd - allocator->mBegin;
}

uint32_t JBKLinAlloc_GetSize(JBKLinAlloc* allocator) { 
	return allocator->mNext - allocator->mBegin; 
}

void* JBKLinAlloc_GetCurrent(JBKLinAlloc* allocator)	{ 
	return allocator->mNext; 
}

// allocates the requested amount of memory if possible
void* JBKLinAlloc_Allocate(JBKLinAlloc* allocator, uint32_t size) {
	// get the next free position
	uint8_t* res = allocator->mNext;

	// apply alignment
	size = AlignedSize(size, allocator->mAlignment);

	// check for buffer overrun
	if (res + size >= allocator->mEnd) {
		ASSERT(FALSE, "Out of memory!");
		return NULL;
	}

#if PARANOIA > 0
	// fill the allocated memory with magic numbers to notice dangling pointers rapidly
	{
		uint32_t* p;
		for (p = (uint32_t*)res; p < (uint32_t*)res + size / sizeof(uint32_t); p++) {
			*p = NONINIT_MEMORY_MAGIC_NUMBER;
		}
	}
#endif

	// advance the next free position
	allocator->mNext += size;
	return res;
}

// frees all memory after the given pointer
JBKLinAllocResult JBKLinAlloc_Rewind(JBKLinAlloc* allocator, void* ptr) {
	// check range
	if ((uint8_t*)ptr >= allocator->mBegin && (uint8_t*)ptr <= allocator->mNext) {
		// apply alignment
		allocator->mNext = AlignedPointer(ptr, allocator->mAlignment);

#if PARANOIA > 0
	// fill the freed memory with magic numbers to notice dangling pointers rapidly
	{
		uint32_t* p;
		for (p = (uint32_t*)ptr; p < (uint32_t*)allocator->mEnd; p++) {
			*p = FREED_MEMORY_MAGIC_NUMBER;
		}
	}
#endif
	} else {
		ASSERT(FALSE, "Tried to rewind to a pointer that is out of range!");
		return JBK_LINALLOC_OUT_OF_RANGE;
	}

	return JBK_LINALLOC_OK;
}

// clears all allocated memory
JBKLinAllocResult JBKLinAlloc_Clear(JBKLinAlloc* allocator) {
	JBKLinAlloc_Rewind(allocator, allocator->mBegin);

	return JBK_LINALLOC_OK;
}

uint32_t JBKLinAlloc_AlignedSize(JBKLinAlloc* allocator, uint32_t size) {
	return AlignedSize(size, allocator->mAlignment);
}
