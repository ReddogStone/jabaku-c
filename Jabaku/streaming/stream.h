#ifndef JBK_STREAM_H
#define JBK_STREAM_H

#include <stdint.h>

typedef struct JBKReadStreamTable {
	uint32_t (*Read)(void* self, int8_t* buffer, int32_t bytesToRead);
	int (*EndOfStream)(void* self);
} JBKReadStreamTable;
typedef JBKReadStreamTable* JBKReadStream;

uint32_t JBKReadStream_Read(JBKReadStream* self, int8_t* buffer, int32_t bytesToRead);
int JBKReadStream_EndOfStream(JBKReadStream* self);

typedef struct JBKWriteStreamTable {
	int (*Write)(void* self, int8_t* data, int32_t bytesToWrite);
} JBKWriteStreamTable;
typedef JBKWriteStreamTable* JBKWriteStream;

#endif // JBK_STREAM_H
