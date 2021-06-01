#include "ParserUniversitario.h"
#include "../SequentialFile/SequentialFile.h"
#include "../Hash/HashIndex.h"
#include <chrono>
string dataFilePath = "datafile.dat";
string auxFilePath = "auxfile.dat";

string indexFilePath = "indexFile.dat";
string bucketFilePath = "bucketFile.dat";
ParserUniversitario pu;

void deleteFilesSQF(){
    remove(dataFilePath.c_str());
    remove(auxFilePath.c_str());
    cout << "Files deleted SQF" << endl;
}

void deleteFilesHash(){
    remove(indexFilePath.c_str());
    remove(bucketFilePath.c_str());
    cout << "Files deleted hash" << endl;
}
// Get information from csv
// ---------------------------------------------------
void testHash(string amount, vector<Universitario>& data){
  cout << "----------------------------------------" << endl;
  cout << "Start Test " << amount <<" - Hash" << endl;
  HashIndex<Universitario, const char *, UniversitarioHash> dinamicHash(indexFilePath, bucketFilePath);
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
  cout << "Finish Test "<< amount<<" - Hash" << endl;
  cout << "----------------------------------------" << endl;
}

void testSQF(string amount, vector<Universitario>& data, int capacity){
  cout << "----------------------------------------" << endl;
  cout << "Start Test " << amount <<" - SQF" << endl;
  SequentialFile<Universitario, const char*> sq(dataFilePath, auxFilePath, capacity);
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
  cout << "Finish Test "<< amount<<" - SQF" << endl;
  cout << "----------------------------------------" << endl;
}

vector<Universitario> getUniversitarioData(string& path){
  string filenamePath = path;
  pu.filename = filenamePath;
  vector<Universitario> answer = pu.getData();
  cout <<"Amount: " <<answer.size() << endl;
  return answer;
}

void TESTALL(){
  vector<string> files = {"../csv/Dataset/1k.csv","../csv/Dataset/5k.csv","../csv/Dataset/10k.csv",
                          "../csv/Dataset/50k.csv","../csv/Dataset/100k.csv"};
  vector<string> amount = {"1k", "5k", "10k", "50k", "100k"};
  vector<int> capacity = {5, 20, 100, 300, 500};
  for(int i = 0; i < files.size(); i++){
    cout <<"-------------------------------------------" << endl;
    vector<Universitario> data = getUniversitarioData(files[i]);
    testSQF(amount[i], data, capacity[i]);
    testHash(amount[i],data);
  }
}

int main(){
  TESTALL();
}

/*
vector<Universitario> getUniversitarios_1k(){
  string filenamePath = "../csv/Dataset/1k.csv";
  pu.filename = filenamePath;
  vector<Universitario> answer = pu.getData();
  cout <<"Amount: " <<answer.size() << endl;
  return answer;
}

vector<Universitario> getUniversitarios_5k(){
  string filenamePath = "../csv/Dataset/5k.csv";
  pu.filename = filenamePath;
  vector<Universitario> answer = pu.getData();
  cout <<"Amount: " <<answer.size() << endl;
  return answer;
}

vector<Universitario> getUniversitarios_10k(){
  string filenamePath = "../csv/Dataset/10k.csv";
  pu.filename = filenamePath;
  vector<Universitario> answer = pu.getData();
  cout <<"Amount: " <<answer.size() << endl;
  return answer;
}

vector<Universitario> getUniversitarios_50k(){
  string filenamePath = "../csv/Dataset/50k.csv";
  pu.filename = filenamePath;
  vector<Universitario> answer = pu.getData();
  cout <<"Amount: " <<answer.size() << endl;
  return answer;
}

vector<Universitario> getUniversitarios_100k(){
  string filenamePath = "../csv/Dataset/100k.csv";
  pu.filename = filenamePath;
  vector<Universitario> answer = pu.getData();
  cout <<"Amount: " <<answer.size() << endl;
  return answer;
}*/
// ---------------------------------------------------