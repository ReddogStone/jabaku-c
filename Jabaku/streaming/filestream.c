#include "filestream.h"

#include "utility/assertion.h"

const uint32_t c_JBKFileReadStream_Size = sizeof(JBKFileReadStream);

static uint32_t JBKFileReadStream_Read(JBKFileReadStream* self, int8_t* buffer, int32_t bytesToRead) {
	size_t bytesRead = fread(buffer, 1, bytesToRead, self->m_FileHandle);
	if (bytesRead != bytesToRead) {
		ASSERT(ferror(self->m_FileHandle) == 0, "Error reading from file!");
	}

	return bytesRead;
}

static int JBKFileReadStream_EndOfStream(JBKFileReadStream* self) {
	return feof(self->m_FileHandle) != 0;
}

static JBKReadStreamTable s_vTable = {JBKFileReadStream_Read, JBKFileReadStream_EndOfStream};

void JBKFileReadStream_Init(JBKFileReadStream* this, char* fileName) {
	errno_t error = fopen_s(&this->m_FileHandle, fileName, "r");
	ASSERT(error == 0, "Failed to open ""%s"" for reading!", fileName);

	this->m_vTablePtr = &s_vTable;
}

void JBKFileReadStream_Destroy(JBKFileReadStream* this) {
	int res = fclose(this->m_FileHandle);

	ASSERT(res == 0, "Could not close the file!");
}

JBKReadStream* JBKFileReadStream_AsReadStream(JBKFileReadStream* self) {
	return (JBKReadStream*)self;
}
