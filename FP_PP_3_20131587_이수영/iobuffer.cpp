#include "iobuffer.h"
#include <string>

/* Generate the IOBuffer with max size of field. */
IOBuffer::IOBuffer(int maxBytes) {
	Init(maxBytes);
}

IOBuffer & IOBuffer::operator=(const IOBuffer & buffer) {
	if (MaxBytes < buffer.BufferSize) return *this; // It fails.
	Initialized = buffer.Initialized;
	BufferSize = buffer.BufferSize;
	memcpy(Buffer, buffer.Buffer, buffer.BufferSize);
	NextByte = buffer.NextByte;
	Packing = Packing;
	return *this;
}

/* Clear the field value from buffer. */
void IOBuffer::Clear() {
	NextByte = 0;
	Packing = TRUE;
}

void IOBuffer::Print(ostream & stream) const {
	stream << "MaxBytes " << MaxBytes << "BufferSize " << BufferSize;
}

int IOBuffer::Init(int maxBytes) {
	Initialized = FALSE;
	if (maxBytes < 0) maxBytes = 0;
	MaxBytes = maxBytes;
	Buffer = new char[MaxBytes];
	BufferSize = 0;
	Clear();
	return 1;
}

// Directly read specific record.
int IOBuffer::DRead(istream &stream, int recref) {
	stream.seekg(recref, ios::beg);
	if ( (int)stream.tellg() != recref) return -1;
	return Read(stream);
}

// Directly write specific record.
int IOBuffer::DWrite(ostream &stream, int recref) const {
	stream.seekp(recref, ios::beg);
	if ( (int)stream.tellp() != recref) return  -1;
	return Write(stream);
}

static const char * headerStr = "IOBuffer";
static const int headerSize = 8;

int IOBuffer::ReadHeader(istream & stream) {
	char* str = new char[headerSize + 1];
	stream.seekg(0, ios::beg);
	stream.read(str, headerSize);
	if (!stream.good()) return -1;
	if (strncmp(str, headerStr, headerSize) == 0)
		return headerSize;
	else return -1;
}

int IOBuffer::WriteHeader(ostream & stream) const {
	stream.seekp(0, ios::beg);
	stream.write(headerStr, headerSize);
	if (!stream.good()) return -1;
	return headerSize;
}