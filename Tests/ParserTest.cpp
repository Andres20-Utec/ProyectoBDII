#include "ParserHash.h"
#include "ParserSQF.h"

void testAll(){

  cout <<"-------------------------------------------" << endl;
  vector<Universitario> vu1k = getUniversitarios_1k();
  testSQF_1k(vu1k);
  testHash_1k(vu1k);
  cout <<"-------------------------------------------" << endl;
  vector<Universitario> vu5k = getUniversitarios_5k();
  testSQF_5k(vu1k);
  testHash_5k(vu1k);
  cout <<"-------------------------------------------" << endl;
  vector<Universitario> vu10k = getUniversitarios_10k();
  testSQF_10k(vu1k);
  testHash_10k(vu1k);
  cout <<"-------------------------------------------" << endl;
  vector<Universitario> vu50k = getUniversitarios_50k();
  testSQF_50k(vu1k);
  testHash_50k(vu1k);
  cout <<"-------------------------------------------" << endl;
  vector<Universitario> vu100k = getUniversitarios_100k();
  testSQF_100k(vu1k);
  testHash_100k(vu1k);
  cout <<"-------------------------------------------" << endl;

}

int main(){
  testAll();
}