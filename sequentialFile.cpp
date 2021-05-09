#include "record.cpp"
#include <algorithm>
#define CAPACITY 5
class SequentialFile
{
private:
    string datafile;
    string auxfile;
    int amount_control;
public:
    SequentialFile(string datafile, string auxfile) : datafile(datafile), auxfile(auxfile){
        amount_control = 0;
    }

    void insertAll(vector<Record> &records)
    {
        sort(records.begin(), records.end(), compareByKey);
        ofstream ofs(this->datafile, ios::binary);
        if(numRecords(this->datafile) == 0){
            int n = records.size();
            for (int i = 0; i < n; i++){
                records[i].nextDel = i + 1;
                ofs << records[i];
            }
            /*records[n - 1].nextDel = -1;
            ofs << records[n - 1];*/
        }else{

        }

        ofs.close();
    }

    int binarySearch(string key)
    {
        fstream inFile(this->datafile, ios::in | ios::binary);
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
        fstream inFile(this->datafile, ios::in | ios::binary);
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
        fstream inFile(this->datafile, ios::in | ios::binary);
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

    vector<Record> reBuild(){
        fstream fsAux("aux.dat", ios::in | ios::out | ios::binary);
        fstream fsData("datos.dat", ios::in | ios::out | ios::binary);
        bool file_switch = false;         // Decide which file read
        int i = 0, nextPosition = 0;      // Select the next position
        vector<Record> records;
        Record temp;
        while (fsAux || fsData){
            if (!file_switch){
                fsData.seekg(sizeof(Record) * nextPosition);
                fsData >> temp;
                if (temp.reference == 'a'){
                    nextPosition = temp.nextDel - 1;
                    file_switch = true;
                }else
                    nextPosition = temp.nextDel;
            }
            else{
                fsAux.seekg(sizeof(Record) * nextPosition);
                fsAux >> temp;
                if (temp.reference == 'd'){
                    nextPosition = temp.nextDel;
                    file_switch = false;
                }
                else
                    nextPosition = temp.nextDel - 1;
            }
            i++;
            temp.nextDel = i;
            temp.reference = 'd';
            if (fsAux.eof() || fsData.eof())
                break;
            records.push_back(temp);
        }
        return records;
    }

    void isFull()
    {
        int amountOfRecords = numRecords(this->auxfile);
        if (amountOfRecords == CAPACITY){
            auto sorted_records = reBuild();
            remove(this->datafile.c_str());
            remove(this->auxfile.c_str());
            ofstream file(this->datafile, ios::binary | ios::app);
            for(auto record : sorted_records)
                file << record;
            file.close();
        }
    }
    void add(Record record)
    {
        isFull();
        fstream fsData("datos.dat", ios::in | ios::out | ios::binary);
        fstream fsAux("aux.dat", ios::out | ios::app | ios::binary);
        // Find the position
        string key = record.nombre;
        // Obtain the current pointer
        int pos = binarySearch(key);
        // check the position of the pointer
        Record temp;
        fsData.seekg(sizeof(Record) * pos);
        fsData >> temp;
        if (strcmp(temp.nombre, record.nombre) > 0)
        {
            pos--;
            fsData.seekg(sizeof(Record) * pos);
            fsData >> temp;
        }
        temp.nextDel = (fsAux.tellp() / sizeof(Record)) + 1;
        temp.reference = 'a';
        // Re write the record
        fsData.seekg(sizeof(Record) * pos);
        fsData << temp;
        // Set the item and Add to fsAux;
        record.reference = 'd';
        record.nextDel = pos + 1;
        fsAux << record;
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