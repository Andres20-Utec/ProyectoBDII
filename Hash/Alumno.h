#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

struct Alumno{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    int ciclo;
    float mensualidad;
    int nextDel;

    void showData(){
        cout << "Codigo      : " << codigo << endl;
        cout << "Nombre      : " << nombre << endl;
        cout << "Apellidos   : " << apellidos << endl;
        cout << "Carrera     : " << carrera << endl;
        cout << "Ciclo       : " << ciclo << endl;
        cout << "Mensualidad : " << mensualidad << endl;
        cout << "NEXTDEL     : " << nextDel << endl;
    }
};