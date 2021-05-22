#include <iostream>
#include <cmath>
#include "GlobalConstants.h"
using namespace std;
// https://www.geeksforgeeks.org/program-to-find-the-next-prime-number/

bool isPrime(int n){
    if (n <= 1)  return false;
    if (n <= 3)  return true;
    if (n%2 == 0 || n%3 == 0) return false;
   
    for (int i=5; i*i<=n; i=i+6)
        if (n%i == 0 || n%(i+2) == 0)
           return false;
   
    return true;
}

int nextPrime(int N){
    if (N <= 1)
        return 2;

    int prime = N;
    bool found = false;
    while (!found) {
        if (isPrime(prime))
            return prime;
        prime++;
    }
}

struct MyHash{
  int numRecords;
  int M;
  double d;
  MyHash(){}
  MyHash(int numRecords, double d){
      this->numRecords = numRecords;
      this->d = d;
      calculateM();
  }
  void setM(int M){
      this->M = M;
  }
  void calculateM(){
      int result = ceil((numRecords/double(PAGESIZE))*(1+d));
      setM(nextPrime(result));
  }
  int operator()(const int& k){
    return k%this->M;
  }
};