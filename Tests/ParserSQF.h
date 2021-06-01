#include "ParserData.h"

void testSQF_1k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 1k - SQF" << endl;
  SequentialFile<Universitario, const char*> sq(dataFilePath, auxFilePath);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    sq.add(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    sq.add(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    sq.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesSQF();
}

void testSQF_5k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 5K - SQF" << endl;
  SequentialFile<Universitario,  const char*> sq(dataFilePath, auxFilePath);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    sq.add(vu[i]);
  }

  for(int i = 0; i < 6; i++){
      auto start  = chrono::steady_clock::now();
      sq.add(vu[i*100]);
      auto end    = chrono::steady_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
      cout <<"Add duration: " <<duration << endl;
  }

  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    sq.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesSQF();
}

void testSQF_10k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 10k - SQF" << endl;
  SequentialFile<Universitario, const char*> sq(dataFilePath, auxFilePath);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    sq.add(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    sq.add(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    sq.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesSQF();
}

void testSQF_50k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 50k - SQF" << endl;
  SequentialFile<Universitario, const char*> sq(dataFilePath, auxFilePath);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    sq.add(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    sq.add(vu[i*100]);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add Duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    sq.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesSQF();
}

void testSQF_100k(vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test 100k - SQF" << endl;
  SequentialFile<Universitario, const char*> sq(dataFilePath, auxFilePath);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    sq.add(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    sq.add(vu[i*100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[i*100].getNombre();
    sq.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }

  deleteFilesSQF();
}

