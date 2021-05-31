#ifndef HASHCONTROLLER_H
#define HASHCONTROLLER_H

#include "Alumno.h"
#include "HashIndex.h"
#include <cstdio>
using namespace std;
string indexFilePath = "/home/theflilux/ProyectoBDII/qtdynamichash/indexFile.dat";
string bucketFilePath = "/home/theflilux/ProyectoBDII/qtdynamichash/bucketFile.dat";
HashIndex<Alumno, const char *, AlumnoHash> test(indexFilePath, bucketFilePath);

class HashController{
public:

    void printTestStart(string s){
        string txt = "******** Test: " + s + " ********";
        cout << txt << endl;
    }
    void printTestEnd(){
        cout << " ********************************" << endl;
    }

    void insertByDefault(string codigo, string nombre, string apellidos, string carrera, int ciclo, float mensualidad){
        cout << R"(Se inserta las keys: "0001", "0002", "0003")" <<endl;
        Alumno student(codigo, nombre, apellidos, carrera, ciclo, mensualidad);
        test.insert(student);
    }

    void insertTest(string codigo, string nombre, string apellidos, string carrera, int ciclo, float mensualidad) {
        printTestStart("insert()");
        insertByDefault(codigo, nombre, apellidos, carrera, ciclo, mensualidad);
        printTestEnd();
    }

    vector<Alumno> searchTest(string codigo){
        printTestStart(R"(search())");
        vector<Alumno> output = test.search(codigo.c_str());
        printTestEnd();
        return output;
    }

    vector<Alumno> searchPerRangeTest(string begin,string end){
        printTestStart(R"(serachPerRange())");
        vector<Alumno> output = test.searchInRange(begin.c_str(), end.c_str());
        printTestEnd();
        return output;
    }


    void deleteTest(string codigo){
        printTestStart("Delete function");
        test.remove(codigo.c_str());
        printTestEnd();
    }

};

#endif // HASHCONTROLLER_H
