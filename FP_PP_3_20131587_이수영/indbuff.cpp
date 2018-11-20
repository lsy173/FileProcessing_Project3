#ifndef INDBUFF_TC
#define INDBUFF_TC

#include "indbuff.h"

template <class keyType>
IndexBuffer<keyType>::IndexBuffer(int maxKeys, int keySize) : FixedFieldBuffer(1 + 2 * maxKeys, sizeof(int) + maxKeys + keySize + maxKeys * sizeof(int)){
	MaxKeys = maxKeys;
	KeySize = keySize;
	AddField(sizeof(int));
	for (int i = 0; i < maxKeys; i++) {
		AddField(KeySize);
		AddField(sizeof(int));
	}
}

template <class keyType>
int IndexBuffer<keyType>::Pack(const SimpleIndex<keyType> & index) {
	int result;
	Clear();
	result = FixedFieldBuffer::Pack(&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++) {
		result = result && FixedFieldBuffer::Pack(&index.Keys[i]);
		result = result && FixedFieldBuffer::Pack(&index.RecAddrs[i]);
	}
	return result;
}

template <class keyType>
int IndexBuffer <keyType>::Unpack(SimpleIndex<keyType> & index) {
	int result;
	result = FixedFieldBuffer::Unpack(&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++) {
		result = result && FixedFieldBuffer::Unpack(&index.Keys[i]);
		result = result && FixedFieldBuffer::Unpack(&index.RecAddrs[i]);
	}
	return result;
}

template <class keyType>
void IndexBuffer<keyType>::Print(ostream & stream) const {
	stream << "IndexBuffer : KeySize " << KeySize
		<< " MaxKeys " << MaxKeys << endl;
	FixedFieldBuffer::Print(stream);
}

#endif