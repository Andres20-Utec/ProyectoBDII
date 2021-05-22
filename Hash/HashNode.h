#include "GlobalConstants.h"

class Node{
protected:
    bool isLeaf;
public:
    Node() {}
    Node(bool isLeaf): isLeaf(isLeaf) {}
};

class HashInternalNode : public Node{
private:
    AddressType left;
    AddressType right;
public:
    HashInternalNode(){}
};

class HashLeafNode : public Node{
private:
    AddressType pagePosition;
public:
    HashLeafNode() {}
    HashLeafNode(AddressType pagePosition): pagePosition(pagePosition) {}
};