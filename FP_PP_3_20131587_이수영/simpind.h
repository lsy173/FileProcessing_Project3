#ifndef SIMPIND_H
#define SIMPIND_H

template <class keyType>
class BTree;

template <class keyType>
class IndexBuffer;

#include <fstream>
using namespace std;

template <class keyType>
class SimpleIndex {
public:
	SimpleIndex(int maxKeys = 100, int unique = 1);
	~SimpleIndex();
	void Clear();
	int Insert(const keyType key, int recAddr);
	int Remove(const keyType key, const int recAddr = -1);
	int Search(const keyType key, const int recAddr = -1, const int exact = 1) const;
	void Print(ostream &) const;
	int numKeys() const { return NumKeys; }
	keyType * Keys;
	int * RecAddrs;
protected:
	int MaxKeys;
	int NumKeys;
	int Find(const keyType key, const int recAddr = -1, const int exact = 1) const;
	int Init(const int maxKeys, const int unique);
	int Unique;
	friend class IndexBuffer<keyType>;
#ifdef BTREE_H
	friend class BTree<keyType>;
#endif
};

#endif