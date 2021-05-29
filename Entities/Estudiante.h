#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

class Estudiante{
private:
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    int ciclo;
    float mensualidad;
public:
    int nextDel;
    char reference;
    Estudiante() = default;
    Estudiante(string codigo, string nombre, string apellidos, string carrera, int ciclo, float mensualidad){
        if(codigo.size() > 5 ) codigo = codigo.substr(0, 5);
        if(nombre.size() > 11) nombre = nombre.substr(0, 11);
        if(apellidos.size() > 20) apellidos = apellidos.substr(0, 20);
        if(carrera.size() > 15) carrera = carrera.substr(0, 15);
        strcpy(this->codigo, codigo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->apellidos, apellidos.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
        this->mensualidad = mensualidad;
        this->nextDel = 0;
    }
    void showData(){
        cout << "Codigo      : " << codigo << endl;
        cout << "Nombre      : " << nombre << endl;
        cout << "Apellidos   : " << apellidos << endl;
        cout << "Carrera     : " << carrera << endl;
        cout << "Ciclo       : " << ciclo << endl;
        cout << "Mensualidad : " << mensualidad << endl;
    }
    bool operator < (Estudiante& other){
        return strcmp(this->nombre, other.nombre) < 0;
    }

    bool operator == (Estudiante& other){
        return strcmp(this->nombre, other.nombre) == 0;
    }

    const char *getPrimaryKey() const {
        return nombre;
    }
    bool equalTo(const char* key){
        return strcmp(this->nombre, key) == 0;
    }

    bool lessThanToKey(const char* key){
        return strcmp(this->nombre, key) < 0;
    }

    bool greatherThanToKey(const char* key){
        return strcmp(this->nombre, key) > 0;
    }

    bool lessThanOrEqualToKey(const char* key){
        return strcmp(this->nombre, key) <= 0;
    }

    bool greatherThanOrEqualToKey(const char* key){
        return strcmp(this->nombre, key) >= 0;
    }
};