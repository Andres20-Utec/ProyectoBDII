#pragma once
#include "page.h"
#include <string.h>

using namespace std;

class HashIndex{
  string id;
  int posPage;
  HashIndex(){}
  
};

class Hash{
  int key;
  public:
  void insertAll(vector<Register>);
  
};