#define PAGESIZE 5

template<typename T>
struct Page{
    Record records[PAGESIZE];
    int count;

    Page(): count(0) {}
};