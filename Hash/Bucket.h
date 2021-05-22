#include "GlobalConstants.h"

template <typename Register>
class Bucket{
private:
    Register records[BUCKETSIZE];
    int nextBucket;
    int nextDel;
    int count;
public:
    Bucket(): nextBucket(-1), nextDel(0), count(0) {}
    explicit Bucket(int nextBucket): nextBucket(nextBucket), nextDel(0), count(0) {}
};