#pragma once
#include "../Entities/Universitario.h"
#include "../Entities/Alumno.h"
#include <sstream>
class ParserUniversitario{
  public:
  string filename;

  vector<Universitario> getData(){
    vector<Universitario> output;
    ifstream fichero(this->filename, ios::in);
    cout << filename << endl;
    for(string linea; getline(fichero, linea);){
      stringstream registro(linea);
      string dato;
      Universitario univ;
      univ.nextDel = 0;
      univ.reference = 'd';
      for(int columna = 0; getline(registro, dato, ';'); ++columna){
        switch(columna){
          case 0:
            univ.setCodigo(dato);
            break;
          case 1:
            univ.setNombre(dato);
            break;
          case 2:
            univ.setCarrera(dato);
            break;
        }
      }
      output.push_back(univ);
    }
    fichero.close();
    return output;
  }
};

class ParserAlumno{
  string filename;
  public:
  ParserAlumno(string filename) : filename(filename){}
  vector<Alumno> getData(){
    vector<Alumno> output;
    fstream fichero(this->filename, ios::in);
    for(string linea; getline(fichero, linea);){
      stringstream registro(linea);
      string dato;
      Alumno alumno;
      for(int columna = 0; getline(registro, dato, ';'); ++columna){
        switch(columna){
          case 0:
            alumno.setCodigo(dato);
            break;
          case 1:
            alumno.setNombre(dato);
            break;
          case 2:
            alumno.setApellidos(dato);
            break;
          case 3:
            alumno.setCarrera(dato);
            break;
          case 4:
            alumno.setCiclo(dato);
            break;
          case 5:
            alumno.setMensualidad(dato);
            break;
        }
      }
      output.push_back(alumno);
    }
    return output;
  }
};
