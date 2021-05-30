#include "../Entities/Alumno.h"
#include "../Hash/HashIndex.h"
#include "../Util/GlobalConstants.h"
#include <functional>
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
    cout << R"(Se inserta las keys: "0001", "0002", "0003, 0004, 0005")" <<endl;
    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    Alumno student2("0002", "Penny", "Vargas Cordero", "Industrial", 2, 2500);
    Alumno student3("0003", "Sheldon", "Cooper Vega", "Mecatronica", 9, 3200);
    Alumno student4("0004", "Gabriela", "García Vega", "Mecatronica", 1, 3200);
    Alumno student5("0005", "Renato", "Rodriguez Vega", "Mecatronica", 2, 3200);
    test.insert(student);
    test.insert(student2);
    test.insert(student3);
    test.insert(student4);
    test.insert(student5);
}

void insertTest() {
    printTestStart("insert()");
    insertByDefault();
    auto output = test.searchInRange("0001", "0005");
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
    auto output = test.search("0004");
    showAll(output);
    printTestEnd();
}

void searchTest(){
    printTestStart(R"(search("0005"))");
    insertByDefault();
    vector<Alumno> output = test.search("0005");
    showAll<Alumno>(output);
    printTestEnd();
}

void searchPerRangeTest(){
    printTestStart(R"(serachPerRange("0001", "0004"))");
    insertByDefault();
    vector<Alumno> output = test.searchInRange("0001", "0004");
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

void specialCase(){
    printTestStart(R"(Remover 2 nodos hojas)");
    Alumno student("0000", "Jose", "Del Monte", "Computacion", 5, 2000);
    Alumno student1("0001", "Ricardo", "Del Sol ", "Computacion", 5, 2000);
    Alumno student2("0003", "Geronimo","Del Aguila", "Bio", 6, 1231);
    Alumno student3("0009", "Geronimo","Del Aguila", "Bio", 6, 1231);
    Alumno student4("0010", "Geronimo","Del Aguila", "Bio", 6, 1231);
    Alumno student5("0012", "Geronimo","Del Aguila", "Bio", 6, 1231);
    Alumno student6("0013", "Geronimo","Del Aguila", "Bio", 6, 1231);
    Alumno student7("0014", "Gerundo","Del Aguila", "Bio", 6, 1231);
    
    test.insert(student);
    test.insert(student1);
    test.insert(student2);
    test.insert(student3);
    test.insert(student4);
    test.insert(student5);
    test.insert(student6);
    test.insert(student7);
    test.insert(student7);

    test.remove("0000");
    test.remove("0001");
    test.remove("0003");
    test.remove("0009");
    test.remove("0010");
    test.remove("0014");
    
    vector<Alumno> output = test.searchInRange("0000", "0014");
    showAll<Alumno>(output);
    printTestEnd();
    // struct AlumnoHash myHash;
    // cout << bitset<MAXHEIGHT>(myHash("0000"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0001"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0003"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0009"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0010"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0012"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0013"))<< endl;
    // cout << bitset<MAXHEIGHT>(myHash("0014"))<< endl;
}

int main(){
    // insertTest();
    // bucketsLinkTest();
    // searchTest();
    // searchPerRangeTest();
    // removeTest();
    specialCase();
}