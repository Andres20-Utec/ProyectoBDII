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
void hashTest(string amount, vector<Universitario>& data, int maxheight){
  cout << "----------------------------------------" << endl;
  cout << "Start Test " << amount <<" - Hash" << endl;
  HashIndex<Universitario, const char *, UniversitarioHash> dinamicHash(indexFilePath, bucketFilePath, maxheight);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    dinamicHash.insert(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    dinamicHash.insert(vu[100]);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[100].getNombre();
    dinamicHash.search(codigo);
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
  }
  deleteFilesHash();
  cout << "Finish Test "<< amount<<" - Hash" << endl;
  cout << "----------------------------------------" << endl;
}

void sqfTest(string amount, vector<Universitario>& data, int capacity){
  cout << "----------------------------------------" << endl;
  cout << "Start Test " << amount <<" - SQF" << endl;
  SequentialFile<Universitario, const char*> sq(dataFilePath, auxFilePath, capacity);
  vector<Universitario> vu = data;
  for(int i = 0; i < vu.size(); i++){
    sq.add(vu[i]);
  }
  for(int i = 0; i < 6; i++){
    auto start  = chrono::steady_clock::now();
    sq.add(vu[100]);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Add Duration: " <<duration << endl;
  }
  for(int i = 0; i < 5; i++){
    auto start  = chrono::steady_clock::now();
    const char* codigo = vu[100].getNombre();
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

void PARSERTEST(){
  vector<string> files = {"../csv/Dataset/1k.csv","../csv/Dataset/5k.csv","../csv/Dataset/10k.csv",
                          "../csv/Dataset/50k.csv","../csv/Dataset/100k.csv"};
  vector<string> amount = {"1k", "5k", "10k", "50k", "100k"};
  vector<int> capacity = {50, 250, 400, 1500, 2000};
  vector<int> height = {7, 9, 10, 12, 14};
  for(int i = 0; i < files.size(); i++){
    cout <<"-------------------------------------------" << endl;
    vector<Universitario> data = getUniversitarioData(files[i]);
    sqfTest(amount[i], data, capacity[i]);
    hashTest(amount[i], data, height[i]);
  }
}

int main(){
  PARSERTEST();
  return 0;
}