#include "recfile.h"
#include "tindbuffer.h"
#include <sstream>
#include <string>
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

template <class RecType>
class TextIndexedFile
{
public:
	void Init(RecType & record);
	int Read(RecType & record); // read next record
	int Read(char * key, RecType & record); // read by key
	int Append(RecType & record);
	int Update(char * oldKey, RecType & record);
	int Delete(RecType & record);
	int Create(const char * name, int mode = ios::in | ios::out);
	int Open(const char * name, int mode = ios::in | ios::out);
	int Close();
	TextIndexedFile(IOBuffer & buffer,
		int keySize, int maxKeys = 100);
	~TextIndexedFile(); // close and delete
protected:
	TextIndex Index;
	BufferFile IndexFile;
	TextIndexBuffer IndexBuffer;
	RecordFile<RecType> DataFile;
	char * FileName; // base file name for file
	int SetFileName(const char * fileName,
		char *& dataFileName, char *& indexFileName);
};

template <class RecType>
void TextIndexedFile<RecType>::Init(RecType & record) {
	int recAddr;
	IndexFile.Delete();
	DataFile.Rewind();
	while ((recAddr = DataFile.Read(record)) != -1) {
		Index.Remove(record.Key());
		Index.Insert(Record.Key, recAddr);
	}
	DataFile.Close();
	return;
}

// template method bodies
template <class RecType>
int TextIndexedFile<RecType>::Read(RecType & record)
{
	return DataFile.Read(record, -1);
}

template <class RecType>
int TextIndexedFile<RecType>::Read(char * key, RecType & record)
{
	int ref = Index.Search(key);
	if (ref < 0) return -1;
	int result = DataFile.Read(record, ref);
	return result;
}

template <class RecType>
int TextIndexedFile<RecType>::Append(RecType & record)
{
	char * key = record.Key();
	int ref = Index.Search(key);
	if (ref != -1) // key already in file
		return -1;
	ref = DataFile.Append(record);
	int result = Index.Insert(record.Key(), ref);
	return ref;
}

template <class RecType>
int TextIndexedFile<RecType>::Update
(char * oldKey, RecType & record) {


	return -1;
}

template <class RecType>
int TextIndexedFile<RecType>::Delete(RecType & record)
{
	char * key = record.Key();
	int ref = Index.Search(key);
	int recAddr;
	if (ref == -1) // key doesn't exist in file
		return -1;
	IndexFile.Delete();
	vector<RecType> r_list;
	vector<RecType> rindex_list;
	while (1) {
		if ((recAddr = DataFile.Read(record)) == -1)
			break;
		if (strcmp(record.Key(), key) == 0)
			continue;
		r_list.push_back(record);
		rindex_list.push_back(recAddr);
	}
	DataFile.Rewind();
	for (int i = 0; i<r_list.size(); i++) {
		Index.Remove(r_list[i].Key());
		Index.Insert(r_list[i].Key(), rindex_list[i]);
		DataFile.Write(r_list[i]);
	}
	DataFile.Close();
	int result = Index.Remove(record.Key());
	return ref;
}


template <class RecType>
int TextIndexedFile<RecType>::SetFileName(const char * fileName,
	char *& dataFileName, char *& indexFileName)
	// generate names for the data file and the index file
{
	if (FileName != 0) // object is already attached to a file
		return 0;
	// set FileName member
	FileName = strdup(fileName);
	// generate real file names
	ostringstream dataName, indexName;
	dataName << FileName << ".dat" << ends;
	indexName << FileName << ".ind" << ends;
	dataFileName = strdup(dataName.str().c_str());
	indexFileName = strdup(indexName.str().c_str());
	return 1;
}

template <class RecType>
int TextIndexedFile<RecType>::Create(const char * fileName, int mode)
// use fileName.dat and fileName.ind
{
	int result;
	char * dataFileName, *indexFileName;
	result = SetFileName(fileName, dataFileName, indexFileName);
	//	cout <<"file names "<<dataFileName<<" "<<indexFileName<<endl;
	if (result == -1) return 0;
	result = DataFile.Create(dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Create(indexFileName, ios::out | ios::in);
	if (!result)
	{
		DataFile.Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	return 1;
}
template <class RecType>
int TextIndexedFile<RecType>::Open(const char * fileName, int mode)
// open data and index file and read index file
{
	int result;
	char * dataFileName, *indexFileName;
	result = SetFileName(fileName, dataFileName, indexFileName);
	if (!result) return 0;
	// open files
	result = DataFile.Open(dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Open(indexFileName, mode);
	if (!result)
	{
		DataFile.Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	// read index into memory
	result = IndexFile.Read();
	if (result != -1)
	{
		result = IndexBuffer.Unpack(Index);
		if (result != -1) return 1;
	}
	// read or unpack failed!
	DataFile.Close();
	IndexFile.Close();
	FileName = 0;
	return 0;
}

template <class RecType>
int TextIndexedFile<RecType>::Close()
{
	int result;
	if (!FileName) return 0; // already closed!
	DataFile.Close();
	IndexFile.Rewind();
	IndexBuffer.Pack(Index);
	result = IndexFile.Write();
	//	cout <<"result of index write: "<<result<<endl;
	IndexFile.Close();
	FileName = 0;
	return 1;
}

template <class RecType>
TextIndexedFile<RecType>::TextIndexedFile(IOBuffer & buffer,
	int keySize, int maxKeys)
	:DataFile(buffer), Index(maxKeys),
	IndexFile(IndexBuffer), IndexBuffer(keySize, maxKeys)
{
	FileName = 0;
}

template <class RecType>
TextIndexedFile<RecType>::~TextIndexedFile()
{
	Close();
}

