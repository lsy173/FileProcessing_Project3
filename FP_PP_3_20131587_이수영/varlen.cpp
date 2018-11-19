#include "varlen.h"
#include <string.h>

VariableLengthBuffer::VariableLengthBuffer(int maxBytes) : IOBuffer (maxBytes) {
	Init();	// construct with a maximum of maxFields.
}

void VariableLengthBuffer::Clear() {
	IOBuffer::Clear();  // clear fields from buffer.
}


/* write the number of bytes in the buffer field definitions.
the record length is represented by an unsigned short value. */
int VariableLengthBuffer::Read(istream& stream) {
	if (stream.eof()) return -1;
	int recaddr = (int)stream.tellg();
	Clear();
	unsigned short bufferSize;
	stream.read((char*)&bufferSize, sizeof(bufferSize));
	if (!stream.good()) { stream.clear(); return -1; }
	BufferSize = bufferSize;
	if (BufferSize > MaxBytes) return -1; // buffer overflow.
	stream.read(Buffer, BufferSize);
	if (!stream.good()) { stream.clear(); return -1; }
	return recaddr;
}

// write the length and buffer into the stream.
int VariableLengthBuffer::Write(ostream& stream) const {
	int recaddr = (int)stream.tellp();
	unsigned short bufferSize;
	bufferSize = BufferSize;
	stream.write((char*)&bufferSize, sizeof(bufferSize));
	if (!stream) return -1;
	stream.write(Buffer, BufferSize);
	if (!stream.good()) return -1;
	return recaddr;
}

const char* headerStr = "Variable";
const int headerSize = 8;

// read the header and check for consistency.
int VariableLengthBuffer::ReadHeader(istream& stream) {
	char str[headerSize + 1];
	int result;

	result = IOBuffer::ReadHeader(stream);	// read the IOBuffer header.
	if (!result) return FALSE;
	stream.read(str, headerSize);			// read the header string.
	if (!stream.good()) return FALSE;
	if (strncmp(str, headerStr, headerSize) != 0) return FALSE;
	return (int)stream.tellg();	// read and check the record description.
}

/* write a buffer header to the beginning of the stream.
	A header consists of the IOBuffer header, header string, 
	Vaiable sized record of length fields  that descibes the file records. */
int VariableLengthBuffer::WriteHeader(ostream & stream) const {
	int result;

	result = IOBuffer::WriteHeader(stream);
	if (!result) return FALSE;
	stream.write(headerStr, headerSize);
	if (!stream.good()) return FALSE;
	return (int)stream.tellp();
}

void VariableLengthBuffer::Print(ostream & stream) const {
	IOBuffer::Print(stream);
}

// construct with a maximum of maxFields.
int VariableLengthBuffer::Init(void) {
	Clear();
	return FALSE;
}