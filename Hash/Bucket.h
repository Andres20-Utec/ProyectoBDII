#include "GlobalConstants.h"
#include <algorithm>
template <typename Register>
class Bucket{
private:
    Register records[BUCKETSIZE];
    int nextBucket;
    int count;
public:
    int nextDel;
    Bucket(): nextBucket(-1), nextDel(0), count(0) {}
    explicit Bucket(int nextBucket): nextBucket(nextBucket), nextDel(0), count(0) {}

    bool isFull(){
        return count == BUCKETSIZE;
    }

    void setNextBucket(int nextPosition){
        this->nextBucket = nextPosition;
    }

    int getNextBucket() const {
        return nextBucket;
    }

    void add(Register record){
        if(count < BUCKETSIZE) {
            records[count] = record;
            ++count;
        }else throw out_of_range("BUCKET LLENO");
    }

    void sortBucket(){
        sort(records, records+count, compareRecords);
    }

    static bool compareRecords(Register& r1, Register& r2){
        return r1 < r2;
    }
    void printAll(){
        cout << "******************" << endl;
        for(int i = 0; i < count; ++i){
            records[i].showData();
            cout << "******************" << endl;
        }
    }
    vector<Register> getRecords(){
        vector<Register> output;
        output.insert(output.begin(), records, records+count);
        return output;
    }
};