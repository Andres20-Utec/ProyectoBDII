#include "Alumno.h"
#include "HashIndex.h"
using namespace std;

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

int main(){
    testInsert2();
}