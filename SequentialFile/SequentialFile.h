#include "../Util/GlobalConstants.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

enum FileID {AUXFILE, DATAFILE};

template <class Register, class Key>
class SequentialFile{
private:
    string dataFilePath;
    string auxFilePath;
    bool empty;

    bool check(int position, fstream& file, FileID id){
        file.seekp(0, ios::end);
        int n = (id == DATAFILE) ?
                ((int) file.tellp() - sizeof(int) - sizeof(char)) / sizeof(Register) :
                (int) file.tellp() / sizeof(Register);
        file.seekg(0, ios::beg);
        return  0 <= position && position <= n;
    }

    void writeRegister(int position, fstream& file, Register& record, FileID id){
        if(!check(position, file, id)){
            cout << "INVALIDO" << endl;
        }

        if(id == DATAFILE)
            file.seekp(position * sizeof(Register) + sizeof(int) + sizeof(char), ios::beg);
        else
            file.seekp(position * sizeof(Register), ios::beg);
        file.write((char*)& record, sizeof(Register));
    }

    void readRegister(int position, fstream& file, Register& record, FileID id){
        if(id == DATAFILE)
            file.seekg(position * sizeof(Register) + sizeof(int) + sizeof(char), ios::beg);
        else
            file.seekg(position * sizeof(Register), ios::beg);
        file.read((char*)& record, sizeof(Register));
    }

    int recordsNumber(fstream& file, FileID id){
        if(file.is_open()){
            file.seekg(0, ios::end);
            int n = (id == DATAFILE) ?
                    ((int) file.tellg() - sizeof(int) - sizeof(char)) / sizeof(Register) :
                    (int) file.tellg() / sizeof(Register);
            file.seekg(0, ios::beg);
            return n;
        }return 0;
    }

    int recordsNumber_(fstream& file, FileID id){
        if(file.is_open()){
            file.seekp(0, ios::end);
            int n = (id == DATAFILE) ?
                    ((int) file.tellp() - sizeof(int) - sizeof(char)) / sizeof(Register) :
                    (int) file.tellp() / sizeof(Register);
            file.seekp(0, ios::beg);
            return n;
        }return 0;
    }

    int recordsNumber(string filename, FileID id){
        fstream file(filename, ios::binary | ios::in);
        auto result = recordsNumber(file, id);
        file.close();
        return result;
    }

    void writeFirstValues(fstream& file, AddressType positionOfTheFirstRecord, char referenceOfTheFirstRecord){
        file.seekp(0, ios::beg);
        file.write((char *)& positionOfTheFirstRecord, sizeof(AddressType));
        file.write((char *)& referenceOfTheFirstRecord, sizeof(char));
    }

    void readFirstValues(fstream& file, AddressType& positionOfTheFirstRecord, char& referenceOfTheFirstRecord){
        file.seekg(0, ios::beg);
        file.read((char *)& positionOfTheFirstRecord, sizeof(AddressType));
        file.read((char *)& referenceOfTheFirstRecord, sizeof(char));
    }

    static bool compareRecords(Register& r1, Register& r2){
        return r1 < r2;
    }

public:
    SequentialFile(string dataFilePath, string auxFilePath){
        this->dataFilePath = dataFilePath;
        this->auxFilePath = auxFilePath;
        empty = recordsNumber(this->dataFilePath, DATAFILE) <= 0;
    }

    void insertAll(vector<Register> &records){
        if(this->empty){
            sort(records.begin(), records.end(), compareRecords);
            empty = false;
            fstream file(this->dataFilePath, ios::binary | ios::out);
            writeFirstValues(file, 0, 'd');
            for (int i = 0; i < records.size(); i++){
                records[i].nextDel = i + 1;
                records[i].reference = 'd';
                writeRegister(i, file, records[i], DATAFILE);
            }
            file.close();
        }else{
            for(int i = 0; i < records.size(); i++)
                add(records[i]);
        }
    }

    AddressType bs_lower_bound(Key searchKey){
        fstream file(this->dataFilePath, ios::binary | ios::in);
        int low = 0;
        int high = recordsNumber(file, DATAFILE);
        int mid;
        while (high > low){
            mid = (high + low) / 2;
            Register record;
            this->readRegister(mid, file, record, DATAFILE);
            if (!record.lessThanToKey(searchKey)){
                high = mid;
            }
            else{
                low = mid + 1;
            }
        }
        file.close();
        return low;
    }

    AddressType bs_upper_bound(Key searchKey){
        fstream file(this->dataFilePath, ios::binary | ios::in);
        int low = 0;
        int high = recordsNumber(file, DATAFILE);
        int mid;
        while (high > low){
            mid = (high + low) / 2;
            Register record;
            this->readRegister(mid, file, record, DATAFILE);
            if(!record.greaterThanToKey(searchKey)){
                low = mid + 1;
            }
            else{
                high = mid;
            }
        }
        file.close();
        return low;

    }

    vector<Register> search(Key key){
        fstream file(this->dataFilePath, ios::binary | ios::in);
        if(recordsNumber(this->auxFilePath, AUXFILE) > 0) reBuild();
        int position = bs_lower_bound(key);
        vector<Register> records;
        Register record;
        int recordsQuantity = recordsNumber(file, DATAFILE);
        while(position < recordsQuantity){
            this->readRegister(position, file, record, DATAFILE);
            if(record.equalTo(key))
                records.push_back(record);
            else
                break;
            ++position;
        }
        return records;
    }

    vector<Register> searchPerRange(Key startKey, Key endKey){
        if(recordsNumber(this->auxFilePath, AUXFILE) > 0) reBuild(); //Evitar el salto de archivos
        fstream file(this->dataFilePath, ios::binary | ios::in);
        vector<Register> beginToEnd;
        int ptrB = bs_lower_bound(startKey);
        int ptrA = bs_upper_bound(endKey) - 1;
        Register record;
        while (ptrB <= ptrA){
            this->readRegister(ptrB, file, record, DATAFILE);
            beginToEnd.push_back(record);
            ptrB++;
        }
        file.close();
        return beginToEnd;
    }

    vector<Register> getSortedRecords(){
        int recordsNumberDatafile = recordsNumber(this->dataFilePath, DATAFILE);
        int recordsNumberAuxfile = recordsNumber(this->auxFilePath, AUXFILE);
        fstream dataFile(this->dataFilePath, ios::binary | ios::in | ios::out);
        fstream auxFile(this->auxFilePath, ios::binary| ios::in | ios::out);
        int positionOfTheFirstRecord;
        char currentReference;
        char nextReference;
        readFirstValues(dataFile, positionOfTheFirstRecord, currentReference);
        bool switchFile = (currentReference != 'd');    // Decide which file read
        int i = 0, nextPosition = positionOfTheFirstRecord;   // Select the next position
        vector<Register> records;
        vector<Register> unordered_records;
        int total = recordsNumberAuxfile + recordsNumberDatafile;
        while (total != i){
            Register record;
            if(!switchFile) this->readRegister(nextPosition, dataFile, record, DATAFILE);
            else this->readRegister(nextPosition, auxFile, record, AUXFILE);
            nextPosition = record.nextDel;
            nextReference = record.reference;
            if(currentReference == 'a')
                unordered_records.push_back(record);
            else{
                ++i;
                record.nextDel = i;
                record.reference = 'd';
                records.push_back(record);
            }
            if(nextReference == 'd'){
                sort(unordered_records.begin(), unordered_records.end(), compareRecords);
                for(auto& r : unordered_records){
                    ++i;
                    r.nextDel = i;
                    r.reference = 'd';
                }
                records.insert(records.end(), unordered_records.begin(), unordered_records.end());
                unordered_records.clear();
            }

            if(nextReference == 'a') switchFile = true;
            else switchFile = false;
            currentReference = nextReference;
        }
        return records;
    }

    void reBuild(){
        auto sorted_records = getSortedRecords();
        remove(this->dataFilePath.c_str());
        remove(this->auxFilePath.c_str());
        fstream file(this->dataFilePath, ios::binary | ios::out);
        writeFirstValues(file, 0, 'd');
        for(int i = 0; i < sorted_records.size(); ++i)
            writeRegister(i, file, sorted_records[i], DATAFILE);
        file.close();
    }

    void isFull(){
        int amountOfRecords = recordsNumber(this->auxFilePath, AUXFILE);
        if (amountOfRecords == CAPACITY){
            reBuild();
        }
    }

    void add(Register record){
        if(this->empty){
            fstream file(this->dataFilePath, ios::binary | ios::out);
            writeFirstValues(file, 0, 'd');
            writeRegister(0, file, record, DATAFILE);
            this->empty = false;
            file.close();
            return;
        }
        isFull();
        fstream auxFile(this->auxFilePath, ios::binary | ios::out | ios::app);
        fstream dataFile(this->dataFilePath, ios::binary | ios::in | ios::out);
        int positionOfTheFirstRecord;
        char referenceOfTheFirstRecord;
        readFirstValues(dataFile, positionOfTheFirstRecord, referenceOfTheFirstRecord);
        int position = bs_upper_bound(record.getPrimaryKey()) - 1;
        if(position == -1){
            record.nextDel = positionOfTheFirstRecord;
            record.reference = referenceOfTheFirstRecord;
            writeFirstValues(dataFile, recordsNumber(this->auxFilePath, AUXFILE), 'a');
        }else {
            Register r1;
            readRegister(position, dataFile, r1, DATAFILE);
            record.nextDel = r1.nextDel;
            record.reference = r1.reference;
            r1.nextDel = recordsNumber(this->auxFilePath, AUXFILE);
            r1.reference = 'a';
            writeRegister(position, dataFile, r1, DATAFILE);
        }
        //writeRegister(recordsNumber(this->auxFilePath, AUXFILE), auxFile, record, AUXFILE);
        auxFile.write((char*)& record, sizeof(Register));
        auxFile.close();
        dataFile.close();
    }

    void printAll(){
        fstream dataFile(this->dataFilePath, ios::binary | ios::in);
        fstream auxFile(this->auxFilePath, ios::binary | ios::in);
        cout << "Data file: " << endl;
        int i = 0;
        int recordsNumberDatafile = recordsNumber(dataFile, DATAFILE);
        int recordsNumberAuxfile = recordsNumber(auxFile, AUXFILE);
        Register r;
        while (i < recordsNumberDatafile){
            readRegister(i, dataFile, r, DATAFILE);
            cout << "******************************" << endl;
            r.showData();
            ++i;
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "Aux file: " << endl;
        i = 0;
        while (i < recordsNumberAuxfile){
            readRegister(i, auxFile, r, AUXFILE);
            cout << "******************************" << endl;
            r.showData();
            ++i;
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        dataFile.close();
        auxFile.close();
    }
};