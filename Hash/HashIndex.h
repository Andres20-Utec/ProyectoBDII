
#include "FreeList.h"
#include <functional>
#include "HashNode.h"
#include "Bucket.h"
using namespace std;

template <class Register, class Key, class Hash = hash<Key> > 
class HashIndex{
private:
    typedef Bucket<Register> Bucket;
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

    void insert(Register record){

    }
};