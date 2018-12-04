#include "btnode.cpp"
#include "indbuff.cpp"
#include "btree.h"
#include <iostream>

const int MaxHeight = 5;
template <class keyType>
BTree<keyType>::BTree(int order, int keySize, int unique) : Buffer(1 + 2 * order, sizeof(int)+ order*keySize + order*sizeof(int)), BTreeFile(Buffer), Root(order) {
	Height = 1;
	Order = order;
	PoolSize = MaxHeight * 2;
	Nodes = new BTNode *[PoolSize];
	BTNode::InitBuffer(Buffer, order);
	Nodes[0] = &Root;
}

template <class keyType>
BTree<keyType>::~BTree() {
	Close();
	delete Nodes;
}

template <class keyType>
int BTree<keyType>::Open(const char * name, int mode) {
	int result;
	result = BTreeFile.Open(name, mode);
	if (!result) return result;
	// load root
	BTreeFile.Read(Root);
	Height = 1;  // find height from BTreeFile!
	return 1;
}

template <class keyType>
int BTree<keyType>::Create(const char * name, int mode) {
	int result;
	result = BTreeFile.Create(name, mode);
	if (!result) return result;
	// append root node
	result = BTreeFile.Write(Root);
	Root.RecAddr = result;
	return result != -1;
}

template <class keyType>
int BTree<keyType>::Close() {
	int result;
	result = BTreeFile.Rewind();
	if (!result) return result;
	result = BTreeFile.Write(Root);
	if (result == -1) return 0;
	return BTreeFile.Close();
}

template <class keyType>
int BTree<keyType>::Insert(const keyType key, const int recAddr) {
	int result; int level = Height - 1;
	int newLargest = 0; keyType prevKey, largestKey;
	BTNode * thisNode, *newNode, *parentNode;
	thisNode = FindLeaf(key);

	// test for special case of new largest key in tree
	if (key > thisNode->LargestKey()) {
		newLargest = 1;
		prevKey = thisNode->LargestKey();
	}

	result = thisNode->Insert(key, recAddr);

	// handle special case of new largest key in tree
	if( newLargest)
		for (int i = 0; i < Height - 1; i++) {
			Nodes[i]->UpdateKey(prevKey, key);
			if (i > 0) Store(Nodes[i]);
		}

	while (result == -1) { // if overflow and not root
		// remember the largest key
		largestKey = thisNode->LargestKey();
		// split the node
		newNode = NewNode();
		thisNode->Split(newNode);
		Store(thisNode);
		Store(newNode);
		level--;  // go up to parent level
		if (level < 0) break;
		// insert newNode into parent of thisNode.
		parentNode = Nodes[level];
		result = parentNode->UpdateKey(largestKey, thisNode->LargestKey());
		result = parentNode->Insert(newNode->LargestKey(), newNode->RecAddr);
		thisNode = parentNode;
	}
	Store(thisNode);
	if (level >= 0) return 1; // insert complete
	// else we just split the root
	int newAddr = BTreeFile.Append(Root); // put previous root into file
	// insert 2 keys in new root node
	Root.Keys[0] = thisNode->LargestKey();
	Root.RecAddrs[0] = newAddr;
	Root.Keys[1] = newNode->LargestKey();
	Root.RecAddrs[1] = newNode->RecAddr;
	Root.NumKeys = 2;
	Height++;
	return 1;
}

template <class keyType>
int BTree<keyType>::Remove(const keyType key, const int recAddr) {
	BTreeNode<char> *DN, *PT;
	DN = FindLeaf(key); //parameter로 받은 key를 통해 삭제하려는 node를 저장.
	bool Large_flag = false;
	keyType Large_key = DN->LargestKey(), Large_key_merge;
	if (Large_key == key)
		Large_flag = true;

	int result = DN->Remove(key, recAddr); //parameter로 받은 key값과 recAddr에 해당하는 node 삭제.
	if (result != -1) {       //삭제려는 노드에 key의 최소개수 이상일 때
		if (Large_flag) {     //key값이 가장 큰 값일 때.
			for (int i = 0; i < Height - 1; i++) {
				Nodes[i]->UpdateKey(Large_key, DN->LargestKey());
				if (i > 0)
					Store(Nodes[i]);
			}
		}
		Store(DN);            //key값이 가장 큰 값이 아닐 때는 Store만 실행.
	}
	else {	//node에 key가 최소개수보다 적게 존재할 때.
		BTreeNode<char> *TN;
		for (int i = 0; i < Nodes[Height - 2]->NumKeys; i++) {
			//삭제하려는 노드의 형제(sibling)중에서 합병 가능한 node가 존재하는지 확인
			TN = Fetch(Nodes[Height - 2]->RecAddrs[i]);
			Large_key_merge = TN->LargestKey();
			if (TN->RecAddr == DN->RecAddr)
				continue;
			if (TN->Merge(DN))	// 합병이 성공되면 더이상의 합병이 필요없기에 반복문에서 탈출.
				break;
		}
		PT = Nodes[Height - 2];  //부모노드 저장.

		if (TN->LargestKey() == DN->Keys[0]) {  //합병한 형제노드의 Largest key가 삭제노드의 가장 작은 키와 같을 때, 즉 왼쪽에 있는 노드와 합병되었을 때
			int temp1, temp2;
			for (int i = 0; i < PT->NumKeys; i++) {
				if (PT->Keys[i] == Large_key)       //부모 노드의 키중 하나가 삭제될 노드의 Largest key일때.
					temp1 = i;
				if (PT->Keys[i] == Large_key_merge) //부모 노드의 키중 하나가 삭제노드가 합병된 형제노드의 Largest key일때.
					temp2 = i;
			}
			PT->RecAddrs[temp1] = PT->RecAddrs[temp2]; //부모노드의 Largest key가 새롭게 갱신.
			PT->Remove(Large_key_merge, recAddr); //부모노드의 Largest node가 새롭게 갱신되었으므로, 이전의 것은 삭제.
		}
		else     //왼쪽에 있는 노드와 합병된 것이 아닐 때
			PT->Remove(Large_key, recAddr);   //부모노드로부터 삭제될 노드에 대한 정보를 삭제.
		Store(PT);
		Store(TN);
	}
	return -1;
}

template <class keyType>
int BTree<keyType>::Search(const keyType key, const int recAddr) {
	BTNode * leafNode;
	leafNode = FindLeaf(key);
	return leafNode->Search(key, recAddr);
}

template <class keyType>
void BTree<keyType>::Print(ostream & stream) {
	stream << "BTree of height " << Height << " is " << endl;
	Root.Print(stream);
	if (Height>1)
		for (int i = 0; i<Root.numKeys(); i++)
		{
			Print(stream, Root.RecAddrs[i], 2);
		}
	stream << "end of BTree" << endl;
}

template <class keyType>
void BTree<keyType>::Print
(ostream & stream, int nodeAddr, int level) {
	BTNode * thisNode = Fetch(nodeAddr);
	stream << "Node at level " << level << " address " << nodeAddr << ' ';
	thisNode->Print(stream);
	if (Height>level)
	{
		level++;
		for (int i = 0; i<thisNode->numKeys(); i++)
		{
			Print(stream, thisNode->RecAddrs[i], level);
		}
		stream << "end of level " << level << endl;
	}
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::FindLeaf(const keyType key)
// load a branch into memory down to the leaf with key
{
	int recAddr, level;
	for (level = 1; level < Height; level++)
	{
		recAddr = Nodes[level - 1]->Search(key, -1, 0);//inexact search
		Nodes[level] = Fetch(recAddr);
	}
	return Nodes[level - 1];
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::NewNode() {
	// create a fresh node, insert into tree and set RecAddr member
	BTNode * newNode = new BTNode(Order);
	int recAddr = BTreeFile.Append(*newNode);
	newNode->RecAddr = recAddr;
	return newNode;
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::Fetch(const int recaddr) {
	// load this node from File into a new BTreeNode
	int result;
	BTNode * newNode = new BTNode(Order);
	result = BTreeFile.Read(*newNode, recaddr);
	if (result == -1) return NULL;
	newNode->RecAddr = result;
	return newNode;
}

template <class keyType>
int BTree<keyType>::Store(BTreeNode<keyType> * thisNode) {
	return BTreeFile.Write(*thisNode, thisNode->RecAddr);
}
