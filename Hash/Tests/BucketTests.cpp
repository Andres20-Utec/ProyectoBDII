#include "../Bucket.h"
#include "../Alumno.h"
using namespace std;

int main(){
    Bucket<Alumno> test;
    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    Alumno student2("0002", "Penny", "Vargas Cordero", "Industrial", 2, 2500);
    Alumno student3("0003", "Sheldon", "Cooper Vega", "Mecatronica", 9, 3200);
    test.add(student2);
    test.add(student3);
    test.add(student);
    test.printAll(); // en orden
}