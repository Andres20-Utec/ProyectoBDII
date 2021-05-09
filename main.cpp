#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 5
using namespace std;

struct Record
{
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;
    int nextDel;
    char reference;

    Record() : nextDel(0), reference('d') {}
    Record(string codigo, string nombre, string carrera, int ciclo) : nextDel(0)
    {
        strcpy(this->codigo, codigo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
        this->reference = 'd';
    }
    void showRecord()
    {
        cout << this->codigo << " " << this->nombre << " " << this->carrera << "  " << this->nextDel << " " << reference << endl;
    }
    void setRecord()
    {
        cout << "Ingresa el codigo: ";
        cin >> this->codigo;
        cout << "Ingresa el nombre: ";
        cin >> this->nombre;
        cout << "Ingresa la carrera: ";
        cin >> this->carrera;
        cout << "Ingresa el ciclo: ";
        cin >> this->ciclo;
    }

    friend istream &operator>>(istream &is, Record &r)
    {
        is.read((char *)&r, sizeof(r));
        return is;
    }
    friend ostream &operator<<(ostream &os, Record &r)
    {
        os.write((char *)&r, sizeof(r));
        return os;
    }
};

bool compareByKey(Record &name1, Record &name2)
{
    if (strcmp(name2.nombre, name1.nombre) > 0)
        return true;
    else
        return false;
}

class SequentialFile
{

public:
    string filename;
    SequentialFile(string filename) : filename(filename) {}

    void insertAll(vector<Record> &records)
    {
        sort(records.begin(), records.end(), compareByKey);
        ofstream ofs(this->filename, ios::binary | ios::app);
        for (int i = 0; i < records.size(); i++)
        {
            records[i].nextDel = i + 1;
            ofs << records[i];
        }
        ofs.close();
    }

    int binarySearch(string key)
    {
        fstream inFile(this->filename, ios::in | ios::binary);
        int l = 0;
        int u; // Amount of records
        char keyName[20];
        int sizeRecord = sizeof(Record);
        inFile.seekg(0, ios::end);
        u = (inFile.tellg() / sizeof(Record)) - 1; // Amount of Records
        strcpy(keyName, key.c_str());
        int m;
        inFile.seekg(0, ios::beg);
        while (u >= l)
        {
            m = (l + u) / 2;
            inFile.seekg(m * sizeRecord);
            Record r;
            inFile >> r;
            if (strcmp(keyName, r.nombre) < 0)
            {
                u = m - 1;
            }
            else if (strcmp(keyName, r.nombre) > 0)
            {
                l = m + 1;
            }
            else
            {
                inFile.seekg(0, ios::beg);
                break;
            }
            inFile.seekg(0, ios::beg);
        }
        inFile.close();
        return m;
    }

    void search(string key)
    {
        fstream inFile(this->filename, ios::in | ios::binary);
        int l = 0;
        int u; // Amount of records
        char keyName[20];
        int sizeRecord = sizeof(Record);
        inFile.seekg(0, ios::end);
        u = (inFile.tellg() / sizeof(Record)) - 1; // Amount of Records
        strcpy(keyName, key.c_str());
        int m;
        inFile.seekg(0, ios::beg);
        while (u >= l)
        {
            m = (l + u) / 2;
            inFile.seekg(m * sizeRecord);
            Record r;
            inFile >> r;
            if (strcmp(keyName, r.nombre) < 0)
            {
                u = m - 1;
            }
            else if (strcmp(keyName, r.nombre) > 0)
            {
                l = m + 1;
            }
            else
            {
                break;
            }
        }
        inFile.seekg(m * sizeof(Record));
        Record r1;
        inFile >> r1;
        inFile.seekg(0, ios::beg);
        inFile.close();
        if (strcmp(r1.nombre, keyName) == 0)
        {
            cout << r1.nombre << " : Position -> " << m << endl;
        }
        else
        {
            cout << "Error :" << r1.nombre << " : Position -> " << m << endl;
        }
        // return r1;
    }
    void searchB(string begin, string end)
    {
        vector<Record> beginToEnd;
        int ptrB = binarySearch(begin);
        int ptrA = binarySearch(end);
        fstream inFile(this->filename, ios::in | ios::binary);
        inFile.seekg(ptrB * sizeof(Record));
        while (ptrB <= ptrA)
        {
            Record r;
            inFile >> r;
            cout << r.nombre << endl;
            beginToEnd.push_back(r);
            ptrB++;
        }
        for (int i = 0; i < beginToEnd.size(); i++)
        {
            beginToEnd[i].showRecord();
        }
        inFile.close();
    }

    void isFull()
    {
        fstream fsAux("aux.dat", ios::in | ios::out | ios::binary);
        fstream fsData("datos.dat", ios::in | ios::out | ios::binary);
        fsAux.seekp(0, ios::end);
        int amountOfRecords = fsAux.tellp() / sizeof(Record);
        vector<Record> sortRegister;
        if (amountOfRecords == CAPACITY)
        {
            bool control = true;         // Decide which file read
            int i = 0, nextPosition = 0; // Select the next position
            while (fsAux || fsData)
            {
                Record temp;
                if (control)
                { // 3 a -> 3 d
                    fsData.seekg(sizeof(Record) * nextPosition);
                    fsData >> temp;
                    if (temp.reference == 'a')
                    {
                        nextPosition = temp.nextDel - 1;
                        control = false;
                    }
                    else
                    {
                        nextPosition = temp.nextDel;
                    }
                }
                else
                {
                    fsAux.seekg(sizeof(Record) * nextPosition);
                    fsAux >> temp;
                    if (temp.reference == 'd')
                    {
                        nextPosition = temp.nextDel;
                        control = true;
                    }
                    else
                    {
                        nextPosition = temp.nextDel - 1;
                    }
                }
                i++;
                temp.nextDel = i;
                temp.reference = 'd';
                if (fsAux.eof() || fsData.eof())
                    break;
                sortRegister.push_back(temp);
            }
            //remove both files
            remove("datos.dat");
            remove("aux.dat");
        }

        if (!sortRegister.empty())
        {
            fstream Data2(filename, ios::app | ios::binary);
            for (int i = 0; i < sortRegister.size(); i++)
            {
                Data2 << sortRegister[i];
            }
            Data2.close();
        }

        return;
    }
    void add(Record record)
    {
        isFull();
        fstream fsData("datos.dat", ios::in | ios::out | ios::binary);
        fstream fsAux("aux.dat", ios::app | ios::out | ios::in | ios::binary);
        // Find the position
        string key = record.nombre;
        // Obtain the current pointer
        int pos = binarySearch(key);
        // check the position of the pointer
        Record temp; // Gabriela 2 a
        fsData.seekg(sizeof(Record) * pos);
        fsData >> temp;
        // Select the position in the main file
        if (strcmp(temp.nombre, record.nombre) > 0)
        {
            pos--;
            fsData.seekg(sizeof(Record) * pos);
            fsData >> temp;
        }
        if (temp.reference == 'd')
        {
            temp.nextDel = (fsAux.tellp() / sizeof(Record)) + 1;
            temp.reference = 'a';
            // Re write the record
            fsData.seekg(sizeof(Record) * pos);
            fsData << temp;
            // Set the item and Add to fsAux;
            record.reference = 'd';
            record.nextDel = pos + 1;
            int lastP = fsAux.tellg();
            fsAux.seekg(0, ios::end);
            fsAux << record;
        }
        else
        {
            // Check if we can put the new record between the records of the main file and aux file
            int nextPosition = temp.nextDel; // 2
            Record auxT;
            fsAux.seekg(sizeof(Record) * (nextPosition - 1)); // Gato 1 a
            fsAux >> auxT;                                    // Gato 1 a
            if (strcmp(auxT.nombre, record.nombre) >= 0)      // gati > gerson
            {
                fsAux.seekp(0, ios::end);
                // Calculate the new position of the record from the main file
                temp.nextDel = (fsAux.tellp() / sizeof(Record)) + 1;
                // Re write the record of the main file
                fsData.seekg(sizeof(Record) * pos);
                fsData << temp;
                // Set the item and Add to fsAux;
                record.nextDel = nextPosition;
                record.reference = 'a';
                fsAux << record;
            }
            else
            {
                int latestPosition = nextPosition - 1; // 1
                do
                {

                    if (auxT.reference == 'a') // Gato 1 a
                    {
                        nextPosition = auxT.nextDel - 1; // 1 - 1 = 0
                        fsAux.seekg(sizeof(Record) * nextPosition);
                        fsAux >> auxT; // gonzalo cs 4 d
                    }
                    else
                    {
                        nextPosition = auxT.nextDel;
                        fsData.seekg(sizeof(Record) * nextPosition);
                        fsData >> auxT; // jorge
                    }
                    if (strcmp(record.nombre, auxT.nombre) > 0) // gerson > gonzalo false
                    {
                        latestPosition = nextPosition; // x
                    }
                    else
                    {
                        fsAux.seekg(sizeof(Record) * latestPosition); // gato 1 a
                        fsAux >> auxT;                                // gato 1 a
                        // SET AND ADD
                        record.nextDel = auxT.nextDel;     // gerson 1 a
                        record.reference = auxT.reference; // a
                        fsAux.seekg(0, ios::end);
                        fsAux << record; // gerson 1 a
                        // set current auxT
                        auxT.nextDel = (fsAux.tellg() / sizeof(Record));
                        auxT.reference = 'a';
                        fstream newAuxFile("aux.dat", ios::in | ios::out | ios::binary);
                        newAuxFile.seekg(sizeof(Record) * latestPosition);
                        newAuxFile << auxT;
                        newAuxFile.close();
                        break;
                    }
                } while (true);
            }
        }

        fsData.close();
        fsAux.close();
    }
    void printea()
    {
        fstream fsData("datos.dat", ios::in | ios::out | ios::binary);
        fstream fsAux("aux.dat", ios::out | ios::in | ios::binary);
        while (fsData)
        {
            Record r;
            fsData >> r;
            if (fsData.eof())
                break;
            r.showRecord();
        }
        cout << endl;
        while (fsAux)
        {
            Record r;
            fsAux >> r;
            if (fsAux.eof())
                break;
            r.showRecord();
        }

        fsData.close();
        fsAux.close();
    }

private:
    void f();
};

int main()
{
    string filename = "datos.dat";
    SequentialFile sf(filename);

    vector<Record> records = {
        Record("P-11", "Abad", "cs", 1),
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
    sort(records.begin(), records.end(), compareByKey);
    sf.insertAll(records);

    // string nombre = "Andrea";
    // sf.search(nombre);
    // string n1 = "Claudia";
    // sf.search(n1);
    // string n2 = "Claza";
    // sf.search(n2);

    sf.add(Record("P-18", "Gonzalo", "cs", 0));
    sf.add(Record("P-18", "Gato", "cs", 0));
    sf.add(Record("P-18", "Rosa", "cs", 8));
    // sf.add(Record("P-18", "Gabriel", "cs", 8));
    sf.printea();
    cout << endl;
    sf.add(Record("P-18", "Gerson", "cs", 8));

    cout << endl;
    sf.add(Record("P-19", "Yetsabella", "cs", 8));
    sf.add(Record("P-18", "Cenia", "cs", 8));
    sf.printea();
    // sf.printea();
    return 0;
}
