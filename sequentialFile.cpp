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
    SequentialFile(string datafile, string auxfile){
        this->datafile = datafile;
        this->auxfile = auxfile;
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

    int bs_lower_bound(string key)
    {
        int low = 0, high = numRecords(this->datafile) - 1, mid;
        fstream inFile(this->datafile, ios::in | ios::binary);
        char keyName[20];
        strcpy(keyName, key.c_str());
        while (high > low)
        {
            mid = (high + low) / 2;
            inFile.seekg(mid * sizeof(Record));
            Record r;
            inFile >> r;
            if (strcmp(keyName, r.nombre) <= 0)
            {
                high = mid;
            }
            else
            {
                low = mid + 1;
            }
        }
        inFile.close();
        return low;
    }

    int bs_upper_bound(string key){
        int low = 0, high = numRecords(this->datafile) - 1, mid;
        fstream inFile(this->datafile, ios::in | ios::binary);
        char keyName[20];
        strcpy(keyName, key.c_str());
        while (high > low)
        {
            mid = (high + low) / 2;
            inFile.seekg(mid * sizeof(Record));
            Record r;
            inFile >> r;
            if (strcmp(keyName, r.nombre) >= 0)
            {
                low = mid + 1;
            }
            else
            {
                high = mid;
            }
        }
        inFile.close();
        return low;

    }

    vector<Record> search(string key)
    {
        reBuild();
        if(key.size() > 20) key = key.substr(0, 20);
        int pos = bs_lower_bound(key);
        fstream inFile(this->datafile, ios::in | ios::binary);
        vector<Record> records;
        Record r1;
        inFile.seekg(pos * sizeof(Record));
        while(!inFile.eof()){
            inFile >> r1;
            if(r1.nombre == key)
                records.push_back(r1);
            else
                break;
        }
        return records;
    }

    vector<Record> search_in_range(string begin, string end)
    { 
        reBuild();
        vector<Record> beginToEnd;
        int ptrB = bs_lower_bound(begin);
        int ptrA = bs_upper_bound(end) - 1;
        fstream inFile(this->datafile, ios::in | ios::binary);
        inFile.seekg(ptrB * sizeof(Record));
        while (ptrB <= ptrA)
        {
            Record r;
            inFile >> r;
            beginToEnd.push_back(r);
            ptrB++;
        }
        inFile.close();
        return beginToEnd;
    }

    vector<Record> get_sorted_records(){
        fstream fsAux(this->auxfile, ios::in | ios::out | ios::binary);
        fstream fsData(this->datafile, ios::in | ios::out | ios::binary);
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
        // Find the position
        string key = record.nombre;
        // Obtain the current pointer
        int pos = bs_upper_bound(key);
        int num_records_aux = numRecords(this->auxfile);
        fstream fsData(this->datafile, ios::in | ios::out | ios::binary);
        ofstream fsAux(this->auxfile, ios::binary | ios::out | ios::app);
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
        temp.nextDel = num_records_aux + 1;
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
    void printAll()
    {
        fstream fsData(this->datafile, ios::in | ios::out | ios::binary);
        fstream fsAux(this->auxfile, ios::out | ios::in | ios::binary);
        cout << "Data file: " << endl;
        while (fsData)
        {
            Record r;
            fsData >> r;
            if (fsData.eof())
                break;
            r.showRecord();
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "Aux file: " << endl;
        while (fsAux)
        {
            Record r;
            fsAux >> r;
            if (fsAux.eof())
                break;
            r.showRecord();
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        fsData.close();
        fsAux.close();
    }
};