#ifndef VARLEN_H
#define VARLEN_H

#include <stdlib.h>
#include <iostream>
#include "iobuffer.h"

/* Abstract class designed to support variable length recrds.
	Fields may be of a variety of types.*/
class VariableLengthBuffer : public IOBuffer {
public:
	VariableLengthBuffer(int MaxBytes = 1000);
	VariableLengthBuffer(const VariableLengthBuffer & buffer)
		: IOBuffer(buffer) {}

	void Clear();
	int Read(istream &);
	int Write(ostream &) const;
	int ReadHeader(istream &);
	int WriteHeader(ostream &) const;
	int PackFixLen(void*, int);
	int PackDelimeted(void*, int);
	int PackLength(void*, int);
	void Print(ostream &) const;
	int SizeOfBuffer() const;	// return current size of buffer.
	int Init();
protected:
};

#endif