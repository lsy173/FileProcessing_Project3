#ifndef DELTEXT_H
#define DELTEXT_H

#include <iostream>
using namespace std;
#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

// Buffer with separated text field. Record variable can be packed to buffer or extracted from buffer.
class DelimTextBuffer
{
public:
	DelimTextBuffer(char Delim = '|', int maxBytes = 1000); // create field with maxFields and the separator '|'.
	void Clear();  // clear fields from buffer.
	int Read(istream &);  
	int Write(ostream &) const;
	int Pack(const char *, int size = -1);  // Set the buffer's next field value.
	int Unpack(char *);  // Extract the buffer's next field value.
	void Print(ostream &) const;
	int Init(char delim, int maxBytes = 1000);

	char Delim;
	char DelimStr[2];  // zero terminated string for Delim.
	char * Buffer;     // sie of packed fields.
	int BufferSize;    // size of packed fields.
	int MaxBytes;       // maximum number of characters in the buffer.
	int NextByte;      // packing / unpacking position in buffer.
};

#endif
