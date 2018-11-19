#include "deltext.h"
#include <string>

DelimTextBuffer::DelimTextBuffer(char delim, int maxBytes) {
	Init(delim, maxBytes); // Create new buffer with maxBytes size of fields.
}

void DelimTextBuffer::Clear() {
	MaxBytes = 0;  // clear the field of buffer.
	BufferSize = 0;
}

int DelimTextBuffer::Read(istream &stream) {
	Clear();
	stream.read((char*)&BufferSize, sizeof(BufferSize));
	if (stream.fail()) return FALSE;
	if (BufferSize > MaxBytes) return FALSE; // It indicates that buffer overflowed.
	stream.read(Buffer, BufferSize);
	return stream.good();
}

int DelimTextBuffer::Write(ostream & stream) const {
	stream.write((char*)&BufferSize, sizeof(BufferSize));
	stream.write(Buffer, BufferSize);
	stream << endl;
	return stream.good();
}

int DelimTextBuffer::Pack(const char * str, int size) {
	short len;  // length of string to be packed.
	if (size >= 0) len = size;
	else len = strlen(str);      // If size is -1, then Use strlen(str) as Delim.
	if (len > strlen(str)) return FALSE;
	int start = NextByte;
	NextByte += len + 1;
	if (NextByte > MaxBytes) return FALSE;
	memcpy(&Buffer[start], str, len);  // Set the next field value of buffer.
	Buffer[start + len] = Delim;
	BufferSize = NextByte;
	return TRUE;
}

// Extract the next field value of buffer.
int DelimTextBuffer::Unpack(char * str) {
	int len = -1;
	int start = NextByte;
	for (int i = start; i < BufferSize; i++) {
		if (Buffer[i] == Delim) {
			len = i - start;
			break;
		}
		if (len == -1) return FALSE;
		NextByte += len + 1;
		if (NextByte > BufferSize) return FALSE;
		strncpy(str, &Buffer[start], len);
		str[len] = 0;
		return TRUE;
	}
}

// Protected members.
void DelimTextBuffer::Print(ostream & stream) const {
	stream << "Buffer has max characters " << MaxBytes << "and Buffer Size " << BufferSize << endl;
}

// Initialize new buffer with maxBytes value.
int DelimTextBuffer::Init(char delim, int maxBytes) {
	Delim = delim;
	DelimStr[0] = Delim;
	DelimStr[1] = 0;
	if (maxBytes < 0) maxBytes = 0;
	MaxBytes = maxBytes;
	Buffer = new char[MaxBytes];
	BufferSize = 0;
	return 1;
}