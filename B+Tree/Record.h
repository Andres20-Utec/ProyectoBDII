#include <iostream>
#include <string.h>
using namespace std;

struct Record
{
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;
    int nextDel;
    char reference;

    Record() : nextDel(0), reference('d') {}
    Record(string codigo, string nombre, string carrera, int ciclo) : nextDel(0), reference('d')
    {
        if(codigo.size() > 5 ) codigo = codigo.substr(0, 5);
        if(nombre.size() > 20) nombre = nombre.substr(0, 20);
        if(carrera.size() > 15) carrera = carrera.substr(0, 15);
        strcpy(this->codigo, codigo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
    }

    void showRecord()
    {
        cout << this->codigo << "-" << this->nombre << "-" << this->carrera << "-" << this->nextDel << "-" << reference << endl;
    }
};