#define DEGREE 5

struct Node{
    int keys[DEGREE-1];
    int children[DEGREE];
    int count;

    Node(): count(0) {}
};