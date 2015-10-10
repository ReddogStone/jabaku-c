#include "streaming/stream.h"

uint32_t JBKReadStream_Read(JBKReadStream* self, int8_t* buffer, int32_t bytesToRead) {
	return (*self)->Read(self, buffer, bytesToRead);
}

int JBKReadStream_EndOfStream(JBKReadStream* self) {
	return (*self)->EndOfStream(self);
}
