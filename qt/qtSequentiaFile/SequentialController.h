#ifndef SEQUENTIALCONTROLLER_H
#define SEQUENTIALCONTROLLER_H

#include "SequentialFile.h"
#include "Universitario.h"

string dataFilePath = "/Users/andresriveros/Documents/GitHub/SequentialFile/qt/qtSequentiaFile/datafile.dat";
string auxFilePath = "/Users/andresriveros/Documents/GitHub/SequentialFile/qt/qtSequentiaFile/auxfile.dat";

SequentialFile<Universitario, const char*> test(dataFilePath, auxFilePath);
class SequentialController{
public:

    void deleteFiles(){
        remove(dataFilePath.c_str());
        remove(auxFilePath.c_str());
    }

    void printTestStart(string s){
        string txt = "******** Test: " + s + " ********";
        cout << txt << endl;
    }
    void printTestEnd(){
        cout << " ********************************" << endl;
    }

    void insertByDefault(string codigo, string nombre, string carrera){
        cout << R"(Se inserta >> )" << endl;
        test.add(Universitario(codigo, nombre, carrera));
    }

    void insertAllTest(){
        printTestStart(R"(insertAll("Andres", "Sagasti", "Claudia"))");
        vector<Universitario> records{
            Universitario("P-11", "Andres", "cs"),
            Universitario("P-12", "Sagasti", "cs"),
            Universitario("P-13", "Claudia", "cs"),
            Universitario("P-42","Daniela","cs"),
            Universitario("P-51","Rosario", "bio"),
            Universitario("P-23","Johana","bio"),
            Universitario("P-78", "Osman", "bio"),
            Universitario("P-32","Jorge", "bio"),
            Universitario("P-26", "Gerson", "cs"),
            Universitario("P-39", "Gabriela", "cs")};

        test.insertAll(records);
        //test.printAll();
        printTestEnd();
    }

    void addTest(string codigo, string nombre, string carrera){
        printTestStart("add()");
        insertByDefault(codigo, nombre, carrera);
        //test.printAll();
        printTestEnd();
    }

    vector<Universitario> searchTest(string nombre){
        printTestStart(R"(search())");
        cout << "Hey" << endl;
        auto universitarios = test.search(nombre.c_str());
        cout << "Baby" << endl;
        cout << universitarios[0].getNombre() << endl;
        //showAll(records);
        printTestEnd();
        return universitarios;
    }

    vector<Universitario> searchPerRange(string begin, string end){
        printTestStart(R"(searchPerRange())");
        auto records = test.searchPerRange(begin.c_str(), end.c_str());
        //showAll(records);
        printTestEnd();
        return records;
    }

    void fullAuxFileTest(){
        printTestStart("auxfile is full");
        cout << "CAPACITY del auxfile >> " << CAPACITY << endl;
        for(int i = 1; i <= CAPACITY + 1; ++i){
            test.add(Universitario("P-18", "Andres", "cs"));
        }
        test.printAll();
        cout << "Luego de agregar un elemento mas, se reconstruye el archivo" << endl;
        test.add(Universitario("P-18", "Andres", "cs"));
        test.printAll();
        printTestEnd();
    }
    void deleteTest(string name){
        printTestStart(R"(delete()");
        test.removeRecord(name.c_str());
    }

    void specialCase(){
        printTestStart("Agregar un nuevo registro que es menor al primero");
        vector<Universitario> records = {
                Universitario("P-11", "Ana", "cs"),
                Universitario("P-72", "Carlos", "cs")};
        test.insertAll(records);
        test.add(Universitario("P-18", "Aa", "cs"));
        test.printAll();
        test.reBuild();
        cout << "Luego de reconstruir el datafile" << endl;
        test.printAll();
        printTestEnd();
    }

    void specialCase2(){
        printTestStart("Agregar un nuevo registro que es mayor que el ultimo");
        vector<Universitario> records = {
                Universitario("P-11", "Ana", "cs"),
                Universitario("P-72", "Carlos", "cs")};
        test.insertAll(records);
        test.add(Universitario("P-18", "Zz", "cs"));
        test.printAll();
        test.reBuild();
        cout << "Luego de reconstruir el archivo" << endl;
        test.printAll();
        printTestEnd();
    }

    /*int main(){
        insertAllTest();
        addTest();
        searchTest();
        searchPerRange();
        fullAuxFileTest();
        specialCase();
        specialCase2();
        return 0;
    }*/

};

#endif // SEQUENTIALCONTROLLER_H
