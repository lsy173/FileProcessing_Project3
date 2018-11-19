#include "buffile.h"

using namespace std;

BufferFile::BufferFile(IOBuffer & from) : Buffer(from)
{

}

int BufferFile::Open(const char * filename, int mode) {
	//if (mode&ios::_Noreplace || mode & ios::trunc) return FALSE;
	if (mode&ios::trunc) return FALSE;

	//File.open(filename, mode | ios::in | ios::_Nocreate | ios::binary);
	File.open(filename, mode | ios::in | ios::binary);
	if (!File.good()) return FALSE;
	File.seekg(0, ios::beg);
	File.seekp(0, ios::beg);
	HeaderSize = ReadHeader();
	if (!HeaderSize) return FALSE;
	File.seekp(HeaderSize, ios::beg);
	File.seekg(HeaderSize, ios::beg);
	return File.good();
}

int BufferFile::Create(const char * filename, int mode) {
	if (!(mode & ios::out)) return FALSE;
	//File.open(filename, mode | ios::in | ios::out | ios::_Noreplace | ios::binary);
	File.open(filename, mode | ios::in | ios::out | ios::binary);

	if (!File.good()) {
		File.close();
		return FALSE;
	}
	HeaderSize = WriteHeader();
	return HeaderSize != 0;
}

int BufferFile::Close(void) {
	File.close();
	return TRUE;
}

int BufferFile::Rewind()
{
	File.seekg(HeaderSize, ios::beg);
	File.seekp(HeaderSize, ios::beg);
	return 1;
}

int BufferFile::Read(int recaddr)
{
	if (recaddr == -1) return Buffer.Read(File);
	else return Buffer.DRead(File, recaddr);
}

int BufferFile::Write(int recaddr) {
	if (recaddr == -1) return Buffer.Write(File);
	else return Buffer.DWrite(File, recaddr);
}

int BufferFile::Append() {
	File.seekp(0, ios::end);
	return Buffer.Write(File);
}

IOBuffer & BufferFile::GetBuffer() {
	return Buffer;
}

int BufferFile::ReadHeader() {
	return Buffer.ReadHeader(File);
}

int BufferFile::WriteHeader() {
	return Buffer.WriteHeader(File);
}