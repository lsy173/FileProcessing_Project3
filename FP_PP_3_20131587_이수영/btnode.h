#ifndef BTNODE_H
#define BTNODE_H

template <class keyType>
class BTree;

#include "simpind.h"
#include "recfile.h"
#include "fixfld.h"

template <class keyType>
class BTreeNode : public SimpleIndex <keyType> {
public:
	BTreeNode(int maxKeys, int unique = 1);
	~BTreeNode();
	// Insert and Remove return
	// 0 for failure
	// -1 for overflow
	// 1 for success
	int Insert(const keyType key, int recAddr);
	int Remove(const keyType key, int recAddr = -1);
	int Search (const keyType key, const int recAddr = -1, const int exact = -1) const;
	void Print(ostream &) const;
	int LargestKey(); // returns value of largest key
	int Split(BTreeNode<keyType> * newNode); // move keys into newNode
	int Merge(BTreeNode<keyType> * fromNode); // move keys from fromNode
	int UpdateKey(keyType oldkey, keyType newKey, int recAddr = -1);
	int Pack(IOBuffer& buffer) const;
	int Unpack(IOBuffer& buffer);
	static int InitBuffer(FixedFieldBuffer& buffer, int maxKeys, int keySize = sizeof(keyType));
	
protected:
	//int MaxKeys;
	//int NumKeys;
	int NextNode; // address of next node at same level.
	int RecAddr; // address of this node in the BTree file
	int MinKeys; // minimum number of keys in a node.
	int MaxBKeys; // maximum number of keys in a node.
	int Init();
	void Clear() { NumKeys = 0; RecAddr = -1; }
	friend class BTree<keyType>;
};

#endif
