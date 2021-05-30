#include "../SequentialFile/SequentialFile.h"
#include "../Entities/Universitario.h"

string dataFilePath = "datafile.dat";
string auxFilePath = "auxfile.dat";

SequentialFile<Universitario, const char*> test(dataFilePath, auxFilePath);

template <class T>
void showAll(vector<T> &records){
    for(auto& r : records){
        cout << "--------------------------" << endl;
        r.showData();
    }
}

void deleteFiles(){
    remove(dataFilePath.c_str());
    remove(auxFilePath.c_str());
}

void printTestStart(string s){
    string txt = "******** Test: " + s + " ********";
    cout << txt << endl;
}
void printTestEnd(){
    deleteFiles();
    cout << " ********************************" << endl;
}

void insertByDefault(){
    cout << R"(Se inserta >> "Andres", "Sagasti", "Claudia")" << endl;
    test.add(Universitario("P-11", "Andres", "cs"));
    test.add(Universitario("P-12", "Sagasti", "cs"));
    test.add(Universitario("P-13", "Claudia", "cs"));
}

void insertAllTest(){
    printTestStart(R"(insertAll("Andres", "Sagasti", "Claudia"))");
    vector<Universitario> records{
        Universitario("P-11", "Andres", "cs"),
        Universitario("P-12", "Sagasti", "cs"),
        Universitario("P-13", "Claudia", "cs")};
    test.insertAll(records);
    test.printAll();
    printTestEnd();
}

void addTest(){
    printTestStart("add()");
    insertByDefault();
    test.printAll();
    printTestEnd();
}

void searchTest(){
    printTestStart(R"(search("Andres"))");
    insertByDefault();
    insertByDefault();
    auto records = test.search("Andres");
    showAll(records);
    printTestEnd();
}

void searchPerRange(){
    printTestStart(R"(searchPerRange("Andres", "Claudia"))");
    insertByDefault();
    insertByDefault();
    auto records = test.searchPerRange("Andres", "Claudia");
    showAll(records);
    printTestEnd();

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

int main(){
    insertAllTest();
    addTest();
    searchTest();
    searchPerRange();
    fullAuxFileTest();
    specialCase();
    specialCase2();
    return 0;
}