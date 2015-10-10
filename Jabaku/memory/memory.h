#ifndef JBK_MEMORY_H
#define JBK_MEMORY_H

#include <stdint.h>

struct JBKContiguousPool;
struct JBKDistributedPool;

typedef enum {
	JBK_MEM_GFX,
	JBK_MEM_SYSTEM,
	JBK_MEM_GENERAL,
	JBK_MEM_ENUM_SIZE
} JBKMemType;

void JBKMem_Init(uint32_t* sizes, uint32_t count);
void JBKMem_Cleanup();

void* JBKMem_Alloc(JBKMemType memType, uint32_t size);
void* JBKMem_AllocArray(JBKMemType memType, uint32_t elemSize, uint32_t elemCount);
void JBKMem_NewLevel(JBKMemType memType);
void JBKMem_ClearLevel(JBKMemType memType);

struct JBKContiguousPool* JBKMem_CreateContiguousPool(JBKMemType memType, uint32_t elemSize, uint32_t elemCount);
struct JBKDistributedPool* JBKMem_CreateDistributedPool(JBKMemType memType, uint32_t elemSize);

#endif // JBK_MEMORY_H