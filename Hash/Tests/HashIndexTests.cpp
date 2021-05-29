#include "../Alumno.h"
#include "../HashIndex.h"
#include <cstdio>
using namespace std;
string indexFilePath = "indexFile.dat";
string bucketFilePath = "bucketFile.dat";
HashIndex<Alumno, const char *, AlumnoHash> test(indexFilePath, bucketFilePath);

template <class T>
void showAll(vector<T> &records){
    for(auto& r : records){
        cout << "--------------------------" << endl;
        r.showData();
    }
}

void deleteFiles(){
    remove(indexFilePath.c_str());
    remove(bucketFilePath.c_str());
}

void printTestStart(string s){
    string txt = "******** Test: " + s + " ********";
    cout << txt << endl;
}
void printTestEnd(){
    deleteFiles();
    cout << " ********************************" << endl;
}

void insertByDefault(){
    cout << R"(Se inserta las keys: "0001", "0002", "0003")" <<endl;
    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    Alumno student2("0002", "Penny", "Vargas Cordero", "Industrial", 2, 2500);
    Alumno student3("0003", "Sheldon", "Cooper Vega", "Mecatronica", 9, 3200);
    test.insert(student);
    test.insert(student2);
    test.insert(student3);
}

void insertTest() {
    printTestStart("insert()");
    insertByDefault();
    auto output = test.searchInRange("0001", "0003");
    showAll(output);
    printTestEnd();
}

void bucketsLinkTest(){
    printTestStart("enlazamiento de buckets");
    cout << R"(Se inserta la key: "0001" * (BUCKETSIZE+1))" << endl;
    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    for(int i = 1; i <= BUCKETSIZE+1; ++i){
        test.insert(student);
    }
    auto output = test.search("0001");
    showAll(output);
    printTestEnd();
}

void searchTest(){
    printTestStart(R"(search("0002"))");
    insertByDefault();
    vector<Alumno> output = test.search("0002");
    showAll<Alumno>(output);
    printTestEnd();
}

void searchPerRangeTest(){
    printTestStart(R"(serachPerRange("0001", "0002"))");
    insertByDefault();
    vector<Alumno> output = test.searchInRange("0001", "0002");
    showAll<Alumno>(output);
    printTestEnd();
}

void removeTest(){
    printTestStart(R"(remove("0002"))");
    insertByDefault();
    test.remove("0002");
    auto output = test.searchInRange("0001", "0004");
    showAll<Alumno>(output);
    printTestEnd();
}

int main(){
    insertTest();
    bucketsLinkTest();
    searchTest();
    searchPerRangeTest();
    removeTest();
}