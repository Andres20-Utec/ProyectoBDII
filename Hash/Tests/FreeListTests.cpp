#include <iostream>
#include "FreeList.h"
#include "Alumno.h"
using namespace std;

void createData(){
    ofstream file;
    file.open("datos1.dat", ios::binary);
    int header = -1;
    file.write((char *) &header, sizeof(header));

    Alumno student;
    strcpy(student.codigo, "0001\0");
    strcpy(student.nombre, "Howard\0");
    strcpy(student.apellidos, "Paredes Zegarra\0");
    strcpy(student.carrera, "Computacion\0");
    student.ciclo = 5;
    student.mensualidad = 2000;
    student.nextDel = 0;
    file.write((char *) &student, sizeof(student));

    strcpy(student.codigo, "0002\0");
    strcpy(student.nombre, "Penny\0");
    strcpy(student.apellidos, "Vargas Cordero\0");
    strcpy(student.carrera, "Industrial\0");
    student.ciclo = 2;
    student.mensualidad = 2500;
    file.write((char *) &student, sizeof(student));

    strcpy(student.codigo, "0003\0");
    strcpy(student.nombre, "Sheldon\0");
    strcpy(student.apellidos, "Cooper Vega\0");
    strcpy(student.carrera, "Mecatronica\0");
    student.ciclo = 9;
    student.mensualidad = 3200;
    file.write((char *) &student, sizeof(student));

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
    Alumno student;
    strcpy(student.codigo, "0004\0");
    strcpy(student.nombre, "Johan\0");
    strcpy(student.apellidos, "Tanta Villanueva\0");
    strcpy(student.carrera, "CS\0");
    student.ciclo = 7;
    student.mensualidad = 1800;
    student.nextDel = 0;
    test.add(student);
    auto students = test.load();
    showStudents(students);
    cout << "**********************************" << endl;
}

void addStudent2(){
    cout << "******** AGREGAR ESTUDIANTE 2********" << endl;
    FreeList<Alumno> test("datos1.dat");
    Alumno student;
    strcpy(student.codigo, "0007\0");
    strcpy(student.nombre, "Javier\0");
    strcpy(student.apellidos, "Gutierrez Ayzanoa\0");
    strcpy(student.carrera, "Industrial\0");
    student.ciclo = 1;
    student.mensualidad = 4000;
    student.nextDel = 0;
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