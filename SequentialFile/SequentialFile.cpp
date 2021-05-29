#include "../Util/GlobalConstants.h"
#include <fstream>
#include <algorithm>
using namespace std;

enum FileID {AUXFILE, DATAFILE};

template <class Register, class Key>
class SequentialFile{
private:
    string dataFilePath;
    string auxFilePath;
    bool empty;
    int amount_control;

    static void writeRegister(int position, fstream& file, Register& record, FileID id){
        if(id == DATAFILE)
            file.seekp(position * sizeof(Register) + sizeof(int) + sizeof(char), ios::beg);
        else
            file.seekp(position * sizeof(Register), ios::beg);
        file.write((char*)& record, sizeof(Register));
    }

    static void readRegister(int position, fstream& file, Register& record, FileID id){
        if(id == DATAFILE)
            file.seekg(position * sizeof(Register) + sizeof(int) + sizeof(char), ios::beg);
        else
            file.seekg(position * sizeof(Register), ios::beg);
        file.read((char*)& record, sizeof(Register));
    }

    static int numberOfRecords(fstream& file, FileID id){
        if(file.is_open()){
            file.seekg(0, ios::end);
            int n = (id == DATAFILE) ?
                    ((int) file.tellg() - sizeof(int) - sizeof(char)) / sizeof(Register) :
                    (int) file.tellg() / sizeof(Register);
            file.seekg(0, ios::beg);
            return n;
        }return 0;
    }

    static void writeFirstValues(fstream& file, AddressType positionOfTheFirstRecord, char referenceOfTheFirstRecord){
        file.seekp(0, ios::beg);
        file.write((char *)& positionOfTheFirstRecord, sizeof(AddressType));
        file.write((char *)& referenceOfTheFirstRecord, sizeof(char));
    }

public:
    SequentialFile(string dataFilePath, string auxFilePath){
        this->dataFilePath = dataFilePath;
        this->auxFilePath = auxFilePath;
        amount_control = 0;
        empty = numRecords(this->datafile, DATAFILE) <= 0;
    }

    void insertAll(vector<Record> &records){
        if(this->empty){
            sort(records.begin(), records.end(), compareByKey);
            empty = false;
            fstream ofs(this->datafile, ios::binary | ios::out);
            initialize_first_values(ofs, 0, 'd');
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
        int low = 0, high = numRecords(this->datafile, DATAFILE) , mid;
        fstream inFile(this->datafile, ios::in | ios::binary);
        char keyName[20];
        strcpy(keyName, key.c_str());
        while (high > low)
        {
            mid = (high + low) / 2;
            Record r;
            this->sf_read(mid, inFile, r, DATAFILE);
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
        int low = 0, high = numRecords(this->datafile, DATAFILE), mid;
        fstream inFile(this->datafile, ios::in | ios::binary);
        char keyName[20];
        strcpy(keyName, key.c_str());
        while (high > low)
        {
            mid = (high + low) / 2;
            Record r;
            this->sf_read(mid, inFile, r, DATAFILE);
            if (strcmp(keyName, r.nombre) >= 0) // keyName >= r.nombre
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
        if(numRecords(this->auxfile, AUXFILE) > 0) reBuild(); //Evitar el salto de archivos
        if(key.size() > 20) key = key.substr(0, 20);
        int pos = bs_lower_bound(key);
        fstream inFile(this->datafile, ios::in | ios::binary);
        vector<Record> records;
        Record r1;
        int num_records_datafile = numRecords(inFile, DATAFILE);
        while(pos < num_records_datafile){
            this->sf_read(pos, inFile, r1, DATAFILE);
            if(r1.nombre == key)
                records.push_back(r1);
            else
                break;
            ++pos;
        }
        return records;
    }

    vector<Record> search_in_range(string start, string end)
    { 
        if(numRecords(this->auxfile, AUXFILE) > 0) reBuild(); //Evitar el salto de archivos
        if(start.size() > 20) start = start.substr(0, 20);
        if(end.size() > 20) end = end.substr(0, 20);
        vector<Record> beginToEnd;
        int ptrB = bs_lower_bound(start);
        int ptrA = bs_upper_bound(end) - 1;
        fstream inFile(this->datafile, ios::in | ios::binary);
        Record r;
        while (ptrB <= ptrA){
            this->sf_read(ptrB, inFile, r, DATAFILE);
            beginToEnd.push_back(r);
            ptrB++;
        }
        inFile.close();
        return beginToEnd;
    }

    vector<Record> get_sorted_records(){
        int num_records_datafile = numRecords(this->datafile, DATAFILE);
        int num_records_auxfile = numRecords(this->auxfile, AUXFILE);
        fstream fsAux(this->auxfile, ios::in | ios::out | ios::binary);
        fstream fsData(this->datafile, ios::in | ios::out | ios::binary);
        int first_position;
        char current_reference;
        char next_reference;
        fsData.read((char *)&first_position, sizeof(int));
        fsData.read((char *)&current_reference, sizeof(current_reference));
        bool file_switch = (current_reference != 'd');         // Decide which file read
        int i = 0, nextPosition = first_position;      // Select the next position
        vector<Record> records;
        vector<Record> unordered_records;
        int total = num_records_auxfile + num_records_datafile;
        while (total != i){
            Record temp;
            if(!file_switch) this->sf_read(nextPosition, fsData, temp, DATAFILE);
            else this->sf_read(nextPosition, fsAux, temp, AUXFILE);
            nextPosition = temp.nextDel;
            next_reference = temp.reference;
            if(current_reference == 'a')
                unordered_records.push_back(temp);
            else{
                ++i;
                temp.nextDel = i;
                temp.reference = 'd';
                records.push_back(temp);
            }
            if(next_reference == 'd'){
                sort(unordered_records.begin(), unordered_records.end(), compareByKey);
                for(auto& r : unordered_records){
                    ++i;
                    r.nextDel = i;
                    r.reference = 'd';
                }
                records.insert(records.end(), unordered_records.begin(), unordered_records.end());
                unordered_records.clear();
            }

            if(next_reference == 'a') file_switch = true;
            else file_switch = false;
            current_reference = next_reference;
        }
        return records;
    }

    void reBuild(){
        auto sorted_records = get_sorted_records();
        remove(this->datafile.c_str());
        remove(this->auxfile.c_str());
        fstream file(this->datafile, ios::binary | ios::out);
        initialize_first_values(file, 0, 'd');
        for(auto& record : sorted_records)
            file << record;
        file.close();
    }

    void isFull()
    {
        int amountOfRecords = numRecords(this->auxfile, AUXFILE);
        if (amountOfRecords == CAPACITY){
            reBuild();
        }
    }
    void add(Record record)
    {
        if(this->empty){
            fstream fsData(this->datafile, ios::binary | ios::out);
            initialize_first_values(fsData, 0, 'd');
            this->empty = false;
            fsData.close();
            return;
        }
        isFull();
        
        string key = record.nombre;
        int first_position;
        char first_reference;
        int pos = bs_upper_bound(key) - 1;
        int num_records_datafile = numRecords(this->datafile, DATAFILE);
        int num_records_auxfile = numRecords(this->auxfile, AUXFILE);
        fstream fsAux(this->auxfile, ios::binary | ios::out | ios::app);
        fstream fsData(this->datafile, ios::binary | ios::in | ios::out);
        fsData.seekg(0, ios::beg);
        fsData.read((char *)&first_position, sizeof(int));
        fsData.read((char *)&first_reference, sizeof(char));
        if(pos == -1){
            record.nextDel = first_position;
            record.reference = first_reference;
            initialize_first_values(fsData, num_records_auxfile, 'a');
        }else {
            Record r1;
            this->sf_read(pos, fsData, r1, DATAFILE);
            record.nextDel = r1.nextDel;
            record.reference = r1.reference;
            r1.nextDel = num_records_auxfile;
            r1.reference = 'a';
            this->sf_write(pos, fsData, r1, DATAFILE);
        }
        fsAux << record;
        fsAux.close();
        fsData.close();
    }
    void printAll()
    {
        fstream fsData(this->datafile, ios::binary | ios::in | ios::out);
        fstream fsAux(this->auxfile, ios::binary | ios::in | ios::out);
        cout << "Data file: " << endl;
        int i = 0;
        int num_records_datafile = numRecords(this->datafile, DATAFILE);
        int num_records_auxfile = numRecords(this->auxfile, AUXFILE);
        Record r;
        while (i < num_records_datafile){
            this->sf_read(i, fsData, r, DATAFILE);
            r.showRecord();
            ++i;
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "Aux file: " << endl;
        i = 0;
        while (i < num_records_auxfile){
            this->sf_read(i, fsAux, r, AUXFILE);
            r.showRecord();
            ++i;
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        fsData.close();
        fsAux.close();
    }
};