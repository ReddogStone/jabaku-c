#ifndef JBK_FILE_STREAM_H
#define JBK_FILE_STREAM_H

#include <stdint.h>
#include <stdio.h>

#include "stream.h"

typedef struct JBKFileReadStream {
	JBKReadStream m_vTablePtr;
	FILE* m_FileHandle;
} JBKFileReadStream;

void JBKFileReadStream_Init(JBKFileReadStream* this, char* fileName);
void JBKFileReadStream_Destroy(JBKFileReadStream* this);
JBKReadStream* JBKFileReadStream_AsReadStream(JBKFileReadStream* self);

#endif // JBK_FILE_STREAM_H
