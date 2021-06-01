#pragma once
#include "../Entities/Universitario.h"
#include "../Hash/HashIndex.h"
#include "ParserData.h"


HashIndex<Universitario, const char *, UniversitarioHash> dinamicHash(indexFilePath, bucketFilePath);

void testHash_1k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 1k - Hash" << endl;
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    dinamicHash.insert(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    dinamicHash.insert(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    dinamicHash.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesHash();
  cout << "Finish Test 1k - Hash" << endl;
  cout << "----------------------------------------" << endl;
}

void testHash_5k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 5k - Hash" << endl;
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    dinamicHash.insert(vu[i]);
  }

  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    dinamicHash.insert(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }

  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    dinamicHash.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesHash();
  cout << "Finish Test 5k - Hash" << endl;
  cout << "----------------------------------------" << endl;
}

void testHash_10k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 10k - Hash" << endl;
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    dinamicHash.insert(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    dinamicHash.insert(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    dinamicHash.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesHash();
  cout << "Finish Test 10k - Hash" << endl;
  cout << "----------------------------------------" << endl;
}

void testHash_50k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 50k - Hash" << endl; 
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    dinamicHash.insert(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    dinamicHash.insert(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add Duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    dinamicHash.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesHash();
  cout << "Finish Test 50k - Hash" << endl;
  cout << "----------------------------------------" << endl;
}

void testHash_100k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 100k - Hash" << endl;
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    dinamicHash.insert(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    dinamicHash.insert(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    dinamicHash.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesHash();
  cout << "Finish Test 100k - Hash" << endl;
  cout << "----------------------------------------" << endl;
}
