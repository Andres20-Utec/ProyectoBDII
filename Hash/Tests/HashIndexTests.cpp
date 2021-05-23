#include "../Alumno.h"
#include "../HashIndex.h"
using namespace std;

template <class T>
void showAll(vector<T> &records){
    for(auto& r : records){
        cout << "--------------------------" << endl;
        r.showData();
    }
}

void testInsert() {
    HashIndex<Alumno, const char *, AlumnoHash> test("indexFile.dat", "bucketFile.dat");
    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    Alumno student2("0002", "Penny", "Vargas Cordero", "Industrial", 2, 2500);
    Alumno student3("0003", "Sheldon", "Cooper Vega", "Mecatronica", 9, 3200);
    test.insert(student);
    test.insert(student2);
    test.insert(student3);
}

void testInsert2(){
    HashIndex<Alumno, const char *, AlumnoHash> test("indexFile.dat", "bucketFile.dat");
    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    for(int i = 0; i <= BUCKETSIZE; ++i){
        test.insert(student);
    }
}

void testSearch(){
    HashIndex<Alumno, const char *, AlumnoHash> test("indexFile.dat", "bucketFile.dat");
    vector<Alumno> output = test.search("0002");
    showAll<Alumno>(output);
}

void testSearchInRange(){
    HashIndex<Alumno, const char *, AlumnoHash> test("indexFile.dat", "bucketFile.dat");
    vector<Alumno> output = test.searchInRange("0001", "0002");
    showAll<Alumno>(output);
}

void testRemove(){
    HashIndex<Alumno, const char *, AlumnoHash> test("indexFile.dat", "bucketFile.dat");
    test.remove("0002");
    auto output = test.searchInRange("0001", "0004");
    showAll<Alumno>(output);
}

void testRemove2(){
    HashIndex<Alumno, const char *, AlumnoHash> test("indexFile.dat", "bucketFile.dat");
    Alumno student3("0003", "Sheldon", "Cooper Vega", "Mecatronica", 9, 3200);
    test.insert(student3);
    test.remove("0001");
    auto output = test.searchInRange("0001", "0004");
    showAll<Alumno>(output);
}

int main(){
    testInsert2();
    testRemove2();
}