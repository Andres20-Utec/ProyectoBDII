#include "sequentialFile.cpp"

int main()
{
    string datafile = "datos.dat";
    string auxfile = "auxfile.dat";
    SequentialFile sf(datafile, auxfile);

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
    sf.printea();
    cout << "------------------" << endl;
    // string nombre = "Andrea";
    // sf.search(nombre);
    // string n1 = "Claudia";
    // sf.search(n1);
    // string n2 = "Claza";
    // sf.search(n2);
    //sf.searchB("Andres", "Sagasti");
    sf.add(Record("P-18", "David", "cs", 0));
    sf.add(Record("P-18", "Thalia", "cs", 0));
    sf.add(Record("P-18", "Moca", "cs", 0));
    sf.add(Record("P-18", "Rosa", "cs", 8));
    sf.add(Record("P-18", "Cenia", "cs", 8));
    sf.add(Record("P-18", "Saba", "cs", 8));
    sf.printea();
    cout << "------------------" << endl;
    sf.reBuild();
    sf.printea();
    return 0;
}