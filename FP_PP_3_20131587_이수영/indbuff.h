#ifndef INDBUFF_H
#define INDBUFF_H

#include "fixfld.h"
#include "simpind.h"

template <class keyType>
class IndexBuffer : public FixedFieldBuffer
{
public:
	IndexBuffer(int maxKeys = 100, int keySize = sizeof(keyType));
	int Pack(const SimpleIndex<keyType> &);
	int Unpack(SimpleIndex<keyType> &);
	void Print(ostream &) const;
protected:
	int MaxKeys;
	int KeySize;
};

#endif