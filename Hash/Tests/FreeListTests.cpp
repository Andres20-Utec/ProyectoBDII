#include <iostream>
#include "../FreeList.h"
#include "../Alumno.h"
using namespace std;

void createData(){
    ofstream file;
    file.open("datos1.dat", ios::binary);
    int header = -1;
    file.write((char *) &header, sizeof(header));

    Alumno student("0001", "Howard", "Paredes Zegarra", "Computacion", 5, 2000);
    file.write((char *) &student, sizeof(Alumno));

    Alumno student2("0002", "Penny", "Vargas Cordero", "Industrial", 2, 2500);
    file.write((char *) &student2, sizeof(Alumno));

    Alumno student3("0003", "Sheldon", "Cooper Vega", "Mecatronica", 9, 3200);
    file.write((char *) &student3, sizeof(Alumno));

    file.close();
}

void showStudents(vector<Alumno> students){
    for(auto s: students){
        cout << "--------------------------" << endl;
        s.showData();
    }
}

void loadFile(){
    cout << "******** CARGA DE DATOS ********" << endl;
    FreeList<Alumno> test("datos1.dat");
    auto students = test.load();
    showStudents(students);
    cout << "********************************" << endl;
}

void addStudent(){
    cout << "******** AGREGAR ESTUDIANTE ********" << endl;
    FreeList<Alumno> test("datos1.dat");
    Alumno student("0004", "Johan", "Tanta Villanueva", "CS", 7, 1800);
    test.add(student);
    auto students = test.load();
    showStudents(students);
    cout << "**********************************" << endl;
}

void addStudent2(){
    cout << "******** AGREGAR ESTUDIANTE 2********" << endl;
    FreeList<Alumno> test("datos1.dat");
    Alumno student("0007", "Javier", "Gutierrez Ayzanoa", "Industrial", 1, 4000);
    test.add(student);
    auto students = test.load();
    showStudents(students);
    cout << "**********************************" << endl;
}

void readStudent(){
    cout << "******** LEER REGISTRO i ********" << endl;
    cout << "******** POSICION: 2" << endl;
    FreeList<Alumno> test("datos1.dat");
    auto student = test.readRecord(2);
    student.showData();
    cout << "**********************************" << endl;
}

void deleteRecord(){
    cout << "******** BORRAR REGISTRO i ********" << endl;
    cout << "******** POSICION: 2" << endl;
    FreeList<Alumno> test("datos1.dat");
    test.deleteRecord(2);
    auto students = test.load();
    showStudents(students);
    cout << "**********************************" << endl;
}

int main(){
    createData();
    loadFile();
    addStudent();
    readStudent();
    deleteRecord();

    addStudent2();
    readStudent();
}