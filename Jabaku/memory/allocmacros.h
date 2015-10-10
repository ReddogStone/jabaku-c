#ifndef JBK_ALLOC_MACROS_H
#define JBK_ALLOC_MACROS_H

#include "memory/memory.h"

#define INSTANCE_ALLOC(memtype, type) ((type*) JBKMem_Alloc(memtype, sizeof(type)))
#define ARRAY_ALLOC(memtype, type, size) ((type*) JBKMem_AllocArray(memtype, sizeof(type), size))
#define FREE(x) (0)

#endif // JBK_MALLOC_MACROS_H
