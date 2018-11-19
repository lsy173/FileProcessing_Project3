#ifndef IOBUFFER_H
#define IOBUFFER_H

#include <stdlib.h>
#include <iostream>

using namespace std;

#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

/*
 It is Abstract class for file buffer. It can pack record variable into buffer or extract it from buffer.
 Can handle Input and output of packed buffer. Buffer can be written to output stream when each field has a value.
 Operations associate the value with field(pack), can bring the value of field(unpack).  
*/

class IOBuffer {
public:
	IOBuffer(int maxBytes = 1000);
	IOBuffer & operator = (const IOBuffer &);
	virtual void Clear();
	virtual int Pack(const void *field, int size = -1) = 0;  // set the buffer's next field values.
	virtual int Unpack(void * field, int size = -1) = 0;    //  extract the buffer's next field values.
	virtual void Print(ostream &) const;
	int Init(int maxBytes);
	// Read and Write operations return the address of the record.
	// Sequential Read/Write operation.
	virtual int Read(istream &) = 0;        // Read from the input stream and fetch into buffer.
	virtual int Write(ostream &) const = 0; // Write the buffer into output stream.
	// Direct Access Read/Write operations.
	virtual int DRead(istream &, int recref);        // Read specific record.
	virtual int DWrite(ostream &, int recref) const;// Write spceific record.

	// These header operations return the number of bytes in the header.   
	virtual int ReadHeader(istream &);        // Read stream, fetch on the buffer.
	virtual int WriteHeader(ostream &) const; // Write buffer to the stream.

protected:
	int Initialized; //TRUE if buffer is initialized
	char* Buffer; //character array to hold field values
	int BufferSize; // sum of the sizes of packed fields
	int MaxBytes; // maximum number of characters in the buffer
	int NextByte; // index of next byte to be packed/unpacked
	int Packing; //TRUE if in packing mode, FALSE, if unpacking
};

// Field Packing operations.(Pack the field into buffers.)
int PackFixed(char * buffer, void * field, int size = -1);
int PackDelimited(char *buffer, void * field, int size = -1);
int PackLength(char * buffer, void * field, int size = -1);

#endif
