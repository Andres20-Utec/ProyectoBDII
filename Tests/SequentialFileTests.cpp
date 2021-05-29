#include "../SequentialFile/SequentialFile.h"
#include "../Entities/Estudiante.h"

string datafile = "datafile.dat";
string auxfile = "auxfile.dat";

SequentialFile<Estudiante, const char*> sf(datafile, auxfile);

void test_insertAll(){
    cout << "**** TEST: Start - insertAll()****" << endl;
    vector<Estudiante> records = {
            Estudiante("P-11", "Andres", "Galves", "cs", 1, 2000),
            Estudiante("P-72", "Sagasti", "Gutierres", "cs", 5, 2000),
            Estudiante("P-33", "Jorge", "Villanueva", "cs", 1, 2300),
            Estudiante("P-74", "Claudia", "Ramirez", "cs", 5, 1001),
            Estudiante("P-25", "Gabriela", "Torres", "cs", 1, 2009),
            Estudiante("P-96", "Carla", "Guerrero", "cs", 5, 2020),
            Estudiante("P-89", "Zora", "Sobrados", "cs", 0, 4500),
            Estudiante("P-79", "Talia", "Ayzanoa", "bio", 2, 2190),
            Estudiante("P-56", "Saori", "Torres", "cs", 4, 5234),
            Estudiante("P-18", "Nozomi", "Sulca", "cs", 5, 1002),
            Estudiante("P-46", "Roxanne", "Temoche", "bio", 6, 2000)};
    sf.insertAll(records);
    sf.printAll();
    cout << "**** TEST: End - insertAll() ****\n" << endl;
}

void test_search(){
    cout << "**** TEST: Start - search() ****" << endl;
    string nombre = "Andres";
    auto records = sf.search(nombre.c_str());
    cout << "Busqueda de la key >> " << nombre << endl;
    for(auto record : records){
        record.showData();
    }
    cout << "**** TEST: End - search() ****\n" << endl;
}

void test_search_in_range(){
    cout << "**** TEST: Start - search_in_range() ****" << endl;
    const char beg[] = "Andres";
    const char end[] = "Carla";
    auto records = sf.searchPerRange(beg, end);
    cout << "Inicio de busqueda >> " << beg << endl;
    cout << "Final de busqueda  >> " << end << endl;
    for(auto record : records){
        record.showData();
    }
    cout << "**** TEST: End - search_in_range() ****\n" << endl;
}

void test_add(){
    cout << "**** TEST: Start - add() ****" << endl;
    cout << "CAPACITY del auxfile >> " << CAPACITY << endl;
    sf.add(Estudiante("P-18", "Andres", "Galves", "cs", 1, 2000));
    sf.add(Estudiante("P-11", "Andres", "Galves", "cs", 1, 2000));
    sf.add(Estudiante("P-11", "Andres", "Galves", "cs", 1, 2000));
    sf.add(Estudiante("P-11", "Andres", "Galves", "cs", 1, 2000));
    sf.add(Estudiante("P-11", "Andres", "Galves", "cs", 1, 2000));
    sf.printAll();
    cout << "Luego de agregar un nuevo archivo cuando el auxfile está lleno" << endl;
    sf.add(Estudiante("P-11", "Andres", "Galves", "cs", 1, 2000));
    sf.printAll();
    cout << "**** TEST: End - add() ****\n" << endl;
}

void test_add2(){
    cout << "**** TEST: Start - add() 2****" << endl;
    // ANTES DEL PRIMER REGISTRO DEL DATAFILE
    vector<Estudiante> records = {
            Estudiante("P-11", "Ana", "Gutierrez", "cs", 1, 1000),
            Estudiante("P-72", "Carlos", "Torres", "cs", 5, 2000)};
    sf.insertAll(records);
    sf.add(Estudiante("P-18", "Aa", "xd", "cs", 8, 4000));
    sf.printAll();
    sf.reBuild();
    sf.printAll();
    cout << "**** TEST: End - add() 2****\n" << endl;
}

void test_add3(){
    cout << "**** TEST: Start - add() 3****" << endl;
    // DESPUES DEL ULTIMO REGISTRO DEL DATAFILE
    vector<Estudiante> records = {
            Estudiante("P-11", "Ana", "Gutierrez", "cs", 1, 4000),
            Estudiante("P-72", "Carlos", "Galvez", "cs", 5, 2300)};
    sf.insertAll(records);
    sf.printAll();
    sf.add(Estudiante("P-18", "Zz", "XD", "cs", 8, 500));
    sf.printAll();
    sf.reBuild();
    sf.printAll();
    cout << "**** TEST: End - add() 3****\n" << endl;
}

int main(){
    test_insertAll();
    test_search();
    test_search_in_range();
    test_add();
    //test_add2();
    //test_add3();
    return 0;
}