#include <iostream>
#include "HashUtil.h"
using namespace std;

int main(){
    MyHash myHash(20, 0.1);
    cout << myHash(23) << endl;
}