#pragma once
#include <iostream>
#include <vector>
using namespace std;
#define  N 5
struct Register{
  string code;

};

struct Page{
  Register records[N];
  int nextPage;
};