#include "GlobalConstants.h"

template <typename Register>
class Page{
private:
  Register records[PAGESIZE];
  int nextPage;
public:
  Page(){}
  Page(int nextPage): nextPage(nextPage) {}
};