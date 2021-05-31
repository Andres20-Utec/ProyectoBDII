#include "../SequentialFile/SequentialFile.h"
#include "../Entities/Universitario.h"

string dataFilePath = "datafile.dat";
string auxFilePath = "auxfile.dat";

SequentialFile<Universitario, const char*> test(dataFilePath, auxFilePath);

template <class T>
void showAll(vector<T> &records){
    cout << "ANTES DE IMPRIMIR " << endl;
    for(auto& r : records){
        cout << "--------------------------" << endl;
        r.showData();
    }
    cout << "Luego de imprimir" << endl;
}

void deleteFiles(){
    remove(dataFilePath.c_str());
    remove(auxFilePath.c_str());
}

void printTestStart(const string& s){
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
    printTestStart(R"(insertAll())");
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

void searchPerRangeTest(){
    printTestStart(R"(searchPerRange("Andres", "Claudia"))");
    insertByDefault();
    insertByDefault();
    auto records = test.searchPerRange("Andres", "Claudia");
    showAll(records);
    printTestEnd();
}

void searchPerRange2Test(){
    printTestStart(R"(Caso 2: searchPerRange("Andres, "Sagasti"))");
    cout << R"(Se inserta >> "Andres", "Sagasti", "Claudia" con insertAll())" << endl;
    vector<Universitario> records{
            Universitario("P-11", "Andres", "cs"),
            Universitario("P-12", "Sagasti", "cs"),
            Universitario("P-13", "Claudia", "cs")};
    test.insertAll(records);
    cout << R"(Se inserta "Andres"*3)" << endl;
    for(int i = 1; i <= 3; ++i){
        test.add(Universitario("P-18", "Andres", "cs"));
    }
    auto out = test.searchPerRange("Andres", "Sagasti");
    showAll(out);
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

void remove1Test(){
    printTestStart(R"(remove("Andres, "Claudia"))");
    insertByDefault();
    test.printAll();
    test.removeRecord("Andres");
    test.removeRecord("Claudia");
    cout << "Luego de remover" << endl;
    test.printAll();
    test.reBuild();
    cout << "Se reconstruye el archivo" << endl;
    test.printAll();
    printTestEnd();
}

void remove2Test(){
    printTestStart(R"(remove("Andres", "Sagasti", "Claudia"))");
    insertByDefault();
    test.printAll();
    test.removeRecord("Andres");
    test.removeRecord("Sagasti");
    test.removeRecord("Claudia");
    cout << "Luego de remover todo" << endl;
    test.printAll();
    insertByDefault();
    test.printAll();
    test.reBuild();
    cout << "Se reconstruye el archivo" << endl;
    test.printAll();
    printTestEnd();
}

void remove3Test(){
    printTestStart(R"(insertAll())");
    vector<Universitario> records{
            Universitario("P-11", "Andres", "cs"),
            Universitario("P-12", "Sagasti", "cs"),
            Universitario("P-13", "Claudia", "cs"),
            Universitario("P-42","Daniela","cs"),
            Universitario("P-51","Pedro", "bio"),
            Universitario("P-23","Johana","bio"),
            Universitario("P-78", "Osman", "bio"),
            Universitario("P-32","Jorge", "bio"),
            Universitario("P-26", "Gerson", "cs"),
            Universitario("P-39", "Gabriela", "cs")};
    test.insertAll(records);
    cout << "INICIO" << endl;
    test.printAll();
    test.removeRecord("Pedro");
    cout << "Luego remuevo Pedro" << endl;
    test.printAll();
    test.add(Universitario("P-51","Pedro", "bio"));
    cout << "Luego insert Pedro" << endl;
    test.printAll();
    test.removeRecord("Pedro");
    cout << "Luego remuevo Pedro" << endl;
    test.printAll();
    test.reBuild();
    cout << "Luego reconstruyo el archivo" << endl;
    test.printAll();
    printTestEnd();
}

void testsWithoutDeleting(){
    insertAllTest();
    addTest();
    searchTest();
    searchPerRangeTest();
    searchPerRange2Test();
    fullAuxFileTest();
    specialCase();
    specialCase2();
}

void testsWithDeleting(){
    remove1Test();
    remove2Test();
    remove3Test();
}

void executeAllTests(){
    testsWithoutDeleting();
    cout << "\n\n\t ********** REMOVE TESTS ********** \n\n" << endl;
    testsWithDeleting();
}
int main(){
    executeAllTests();
    return 0;
}