#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

class Alumno{
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
    Alumno() = default;
    Alumno(string codigo, string nombre, string apellidos, string carrera, int ciclo, float mensualidad){
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
    bool operator < (Alumno& other){
        return strcmp(this->codigo, other.codigo) < 0;
    }

    bool operator == (Alumno& other){
        return strcmp(this->codigo, other.codigo) == 0;
    }

    const char *getPrimaryKey() const {
        return codigo;
    }
    bool equalToKey(const char* key){
        return strcmp(this->codigo, key) == 0;
    }

    bool lessThanToKey(const char* key){
        return strcmp(this->codigo, key) < 0;
    }

    bool greaterThanToKey(const char* key){
        return strcmp(this->codigo, key) > 0;
    }

    bool lessThanOrEqualToKey(const char* key){
        return strcmp(this->codigo, key) <= 0;
    }

    bool greaterThanOrEqualToKey(const char* key){
        return strcmp(this->codigo, key) >= 0;
    }

    string getCodigo()const {
            return this->codigo;
    }

    string getNombre()const {
            return this->nombre;
    }

    string getApellidos()const {
            return this->apellidos;
    }

    string getCarrera()const {
            return this->carrera;
    }

    int getCiclo()const {
            return this->ciclo;
    }

    float getMensualidad()const {
            return this->mensualidad;
    }

    void setCodigo(string codigo){
        strcmp(this->codigo, codigo.c_str());
    }

    void setNombre(string nombre){
        strcmp(this->codigo, nombre.c_str());
    }

    void setApellidos(string apellido){
        strcmp(this->apellidos, apellido.c_str());
    }

    void setCarrera(string carrera){
        strcmp(this->carrera, carrera.c_str());
    }

    void setCiclo(string ciclo){
        this->ciclo = stoi(ciclo);
    }

    void setMensualidad(string mensualidad){
        this->mensualidad = stof(mensualidad);
    }

};

struct AlumnoHash{
    int operator()(const char* key){
        string str(key);
        hash<string> myHash;
        return myHash(str);
    }
};