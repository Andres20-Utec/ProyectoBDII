
#include <functional>
#include <bitset>
#include "FreeList.h"
#include "HashNode.h"
#include "Bucket.h"
using namespace std;

template <class Register, class Key, class Hash = hash<Key> > 
class HashIndex{
private:
    typedef Bucket<Register> Bucket;
    typedef bitset<MAXHEIGHT> bitset;
    FreeList<Bucket> bucketFile;
    FreeList<HashNode> indexFile;
    Hash myHash;

    static int numberOfRecords(const string& filename){
        ifstream file(filename, ios::binary);
        if(file.is_open()){
            return 1;
        }
            return 0;
    }
public:
    HashIndex()= default;
    HashIndex(const string& indexFilePath, const string& bucketFilePath){
        this->bucketFile.setPath(bucketFilePath);
        this->indexFile.setPath(indexFilePath);
    }
    void initializeFirstValues(){
        HashNode root(1, 2), v1(0), v2(1);
        Bucket b1, b2;
        this->bucketFile.add(b1);
        this->bucketFile.add(b2);
        this->indexFile.add(root);
        this->indexFile.add(v1);
        this->indexFile.add(v2);
    }

    void insertAll(vector<Register> records){
        if(bucketFile.getNumberOfRecords() == 0)
            initializeFirstValues();

        for(auto& record : records){
            insert(record);
        }
    }

    AddressType searchNode(AddressType nodePosition, bitset key, int& height){
        HashNode node = indexFile.readRecord(nodePosition);
        if(node.isLeaf)
            return nodePosition;
        else{
            if(key[height] == 0) return searchNode(node.left, key, ++height);
            else return searchNode(node.right, key, ++height);
        }
    }

    void insert(Register record){
        if(bucketFile.getNumberOfRecords() == 0)
            initializeFirstValues();
        int height = 0;
        bitset hashKey = myHash(record.getPrimaryKey());
        AddressType currentNodePosition = searchNode(0, hashKey, height);
        HashNode currentNode = indexFile.readRecord(currentNodePosition);
        AddressType currentBucketPosition = currentNode.getBucketPosition();
        Bucket bucket = bucketFile.readRecord(currentBucketPosition);
        if(!bucket.isFull()){
            bucket.add(record);
            bucket.sortBucket();
            bucketFile.writeRecord(currentBucketPosition, bucket);
        }else
            split(record, height, currentNodePosition, currentNode, currentBucketPosition, bucket);
    }

    void split(Register& record, int height, int currentNodePosition, HashNode& currentNode, int currentBucketPosition,
               Bucket& bucket) {
        if(height == MAXHEIGHT){
            Bucket bucketToLink;
            bucketToLink.add(record);
            bucketToLink.setNextBucket(currentBucketPosition);
            currentNode.bucketPosition = bucketFile.add(bucketToLink);
            indexFile.writeRecord(currentNodePosition, currentNode);
            return;
        }
        vector<Register> records = bucket.getRecords();
        records.push_back(record);
        Bucket bucket1, bucket2;
        bool splitAgain = false;
        bool splitToLeft = false;
        for(auto& r : records){
            bitset hashKey = myHash(r.getPrimaryKey());
            if(hashKey[height] == 0){
                if(!bucket1.isFull()) bucket1.add(r);
                else{
                    splitAgain = true;
                    splitToLeft = true;
                }
            }
            else{
                if(!bucket2.isFull()) bucket2.add(r);
                else splitAgain = true;
            }
        }
        bucketFile.deleteRecord(currentBucketPosition);
        AddressType bucketPosition1 = bucketFile.add(bucket1);
        AddressType bucketPosition2 = bucketFile.add(bucket2);
        HashNode leftNode(bucketPosition1), rightNode(bucketPosition2);
        currentNode.left = indexFile.add(leftNode);
        currentNode.right = indexFile.add(rightNode);
        currentNode.isLeaf = false;
        indexFile.writeRecord(currentNodePosition, currentNode);
        if(splitAgain){
            AddressType splitNodePosition, splitBucketPosition;
            HashNode splitNode;
            if(splitToLeft) {
                splitNodePosition = currentNode.left;
                splitNode = indexFile.readRecord(splitNodePosition);
                split(record, height+1, splitNodePosition, splitNode, bucketPosition1, bucket1);
            }else{
                splitNodePosition = currentNode.right;
                splitNode = indexFile.readRecord(splitNodePosition);
                split(record, height+1, splitNodePosition, splitNode, bucketPosition2, bucket2);
            }
        }
    }
};