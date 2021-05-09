#include "sequentialFile.cpp"
string datafile = "datafile.dat";
string auxfile = "auxfile.dat";
SequentialFile sf(datafile, auxfile);

void test_insertAll(){
    cout << "**** TEST: Start - insertAll()****" << endl;
    vector<Record> records = {
        Record("P-11", "Andres", "cs", 1),
        Record("P-72", "Sagasti", "cs", 5),
        Record("P-33", "Jorge", "cs", 1),
        Record("P-74", "Claudia", "cs", 5),
        Record("P-25", "Gabriela", "cs", 1),
        Record("P-96", "Carla", "cs", 5),
        Record("P-89", "Zora", "cs", 0),
        Record("P-79", "Talía", "bio", 2),
        Record("P-56", "Saori", "cs", 5),
        Record("P-18", "Nozomi", "cs", 0),
        Record("P-46", "Roxanne", "bio", 2)};
    sf.insertAll(records);
    sf.printAll();
    cout << "**** TEST: End - insertAll() ****\n" << endl;
}

void test_search(){
    cout << "**** TEST: Start - search() ****" << endl;
    string nombre = "Andres";
    auto records = sf.search(nombre);
    cout << "Busqueda de la key >> " << nombre << endl;
    for(auto record : records){
        record.showRecord();
    }
    cout << "**** TEST: End - search() ****\n" << endl;
}

void test_search_in_range(){
    cout << "**** TEST: Start - search_in_range() ****" << endl;
    string beg = "Andres";
    string end = "Carla";
    auto records = sf.search_in_range(beg, end);
    cout << "Inicio de busqueda >> " << beg << endl;
    cout << "Final de busqueda  >> " << end << endl;
    for(auto record : records){
        record.showRecord();
    }
    cout << "**** TEST: End - search_in_range() ****\n" << endl;
}

void test_add(){
    cout << "**** TEST: Start - add() ****" << endl;
    cout << "CAPACITY del auxfile >> " << CAPACITY << endl;
    sf.add(Record("P-18", "David", "cs", 0));
    sf.add(Record("P-18", "Thalia", "cs", 0));
    sf.add(Record("P-18", "Moca", "cs", 0));
    sf.add(Record("P-18", "Rosa", "cs", 8));
    sf.add(Record("P-18", "Cenia", "cs", 8));
    sf.printAll();
    cout << "Luego de agregar un nuevo archivo cuando el auxfile está lleno" << endl;
    sf.add(Record("P-18", "Saba", "cs", 8));
    sf.printAll();
    cout << "**** TEST: End - add() ****\n" << endl;
}

int main()
{
    test_insertAll();
    test_search();
    test_search_in_range();
    test_add();
    return 0;
}