#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

class Universitario{
private:
    char codigo[5];
    char nombre[15];
    char carrera[15];

public:
    int nextDel;
    char reference;
    Universitario() = default;
    Universitario(string codigo, string nombre, string carrera){
        if(codigo.size() > 5 ) codigo = codigo.substr(0, 5);
        if(nombre.size() > 11) nombre = nombre.substr(0, 11);
        if(carrera.size() > 15) carrera = carrera.substr(0, 15);
        strcpy(this->codigo, codigo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->nextDel = 0;
        this->reference = 'd';
    }
    void showData(){
        cout << "Codigo      : " << codigo << endl;
        cout << "Nombre      : " << nombre << endl;
        cout << "Carrera     : " << carrera << endl;
        //cout << "NextDel     : " << nextDel << endl;
        //cout << "Reference   : " << reference << endl;
    }
    bool operator < (Universitario& other){
        return strcmp(this->nombre, other.nombre) < 0;
    }

    bool operator == (Universitario& other){
        return strcmp(this->nombre, other.nombre) == 0;
    }

    const char *getPrimaryKey() const {
        return nombre;
    }
    bool equalToKey(const char* key){
        return strcmp(this->nombre, key) == 0;
    }

    bool lessThanToKey(const char* key){
        return strcmp(this->nombre, key) < 0;
    }

    bool greaterThanToKey(const char* key){
        return strcmp(this->nombre, key) > 0;
    }

    bool lessThanOrEqualToKey(const char* key){
        return strcmp(this->nombre, key) <= 0;
    }

    bool greaterThanOrEqualToKey(const char* key){
        return strcmp(this->nombre, key) >= 0;
    }
    void setCodigo(string codigo){
        strcpy(this->codigo, codigo.c_str());
    }

    void setNombre(string nombre){
        strcpy(this->nombre, nombre.c_str());
    }
    
    void setCarrera(string carrera){
        strcpy(this->carrera, carrera.c_str());
    }

    const char* getNombre(){
        return this->nombre;
    }
    const char* getCodigo(){
        return this->codigo;
    }
};

struct UniversitarioHash{
    int operator()(const char* key){
        string str(key);
        hash<string> myHash;
        return myHash(str);
    }
};