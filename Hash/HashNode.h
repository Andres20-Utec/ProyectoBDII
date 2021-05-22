#include "GlobalConstants.h"
#include <functional>

class HashNode{
protected:
    AddressType left;
    AddressType right;
    AddressType bucketPosition;
    bool isLeaf;
    int nextDel;
public:
    HashNode() = default;;
    HashNode(AddressType left, AddressType right){
        this->left = left;
        this->right = right;
        this->isLeaf = false;
    }
    HashNode(AddressType bucketPosition){
        this->bucketPosition = bucketPosition;
        this->isLeaf = true;
    }
    template <class Register, class Key, class Hash = std::hash<Key> >
    class HashIndex;
};