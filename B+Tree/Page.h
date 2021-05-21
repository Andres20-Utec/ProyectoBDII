#define PAGESIZE 5

template<typename Record>
struct Page{
    Record records[PAGESIZE];
    int count;

    Page(): count(0) {}
};
