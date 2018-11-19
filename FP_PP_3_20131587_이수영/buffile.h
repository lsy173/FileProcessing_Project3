#ifndef BUFFILE_H
#define BUFFILE_H

#include <stdlib.h>
#include <fstream>
#include "iobuffer.h"

#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

class BufferFile
{
public:
	BufferFile(IOBuffer &);

	int Open(const char* filename, int MODE); // Open already existing file.
	int Create(const char* filename, int MODE);// Create new file.
	int Close(); // Close opened file.
	int Rewind(); // Reconfigure the first data record.
	int Read(int recaddr = -1); //  Read the record into buffer.
	int Write(int recaddr = -1); // write the current buffer into record.
	int Append();  // Record the current buffer to the end of file.

	// Access to IOBuffer.
	IOBuffer & GetBuffer();

protected:
	IOBuffer & Buffer;
	fstream File;
	int HeaderSize;
	int ReadHeader();
	int WriteHeader();
};
#endif