#pragma once
#include "../Parser/ParserUniversitario.h"
#include "../SequentialFile/SequentialFile.h"
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
}
// ---------------------------------------------------