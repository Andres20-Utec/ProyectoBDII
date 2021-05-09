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
        if(numRecords(this->datafile) == 0){
            ofstream ofs(this->datafile, ios::binary);
            for (int i = 0; i < records.size(); i++){
                records[i].nextDel = i + 1;
                ofs << records[i];
            }
            ofs.close();
        }else{
            for(int i = 0; i < records.size(); i++)
                add(records[i]);
        }
    }

    int binarySearch(string key)
    {
        fstream inFile(this->datafile, ios::in | ios::binary);
        inFile.seekg(0, ios::beg);
        int low = 0, high = numRecords(this->datafile) - 1, mid;
        char keyName[20];
        strcpy(keyName, key.c_str());
        while (high >= low)
        {
            mid = (high + low) / 2;
            inFile.seekg(mid * sizeof(Record));
            Record r;
            inFile >> r;
            if (strcmp(keyName, r.nombre) < 0)
            {
                high = mid - 1;
            }
            else if (strcmp(keyName, r.nombre) > 0)
            {
                low = mid + 1;
            }
            else
            {
                break;
            }
        }
        inFile.close();
        return mid;
    }

    bool search(string key)
    {
        int pos = binarySearch(key);
        fstream inFile(this->datafile, ios::in | ios::binary);
        Record r1;

        inFile.seekg(pos * sizeof(Record));
        inFile >> r1;
        inFile.close();

        if (strcmp(r1.nombre, key.c_str()) == 0)
        {
            cout << r1.nombre << " : Position -> " << pos << endl;
            return true;
        }
        else
        {
            cout << "Error :" << r1.nombre << " : Position -> " << pos << endl;
            return false;
        }
    }

    void search_in_range(string begin, string end)
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

    vector<Record> get_sorted_records(){
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

    void reBuild(){
        auto sorted_records = get_sorted_records();
        remove(this->datafile.c_str());
        remove(this->auxfile.c_str());
        ofstream file(this->datafile, ios::binary);
        for(auto record : sorted_records)
            file << record;
        file.close();
    }

    void isFull()
    {
        int amountOfRecords = numRecords(this->auxfile);
        if (amountOfRecords == CAPACITY){
            reBuild();
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
        temp.nextDel = numRecords(this->auxfile) + 1;
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
};