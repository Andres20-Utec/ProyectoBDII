#ifndef SEQUENTIALFILE_H
#define SEQUENTIALFILE_H

#include "GlobalConstants.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

enum FileID {AUXFILE, DATAFILE};

template <class Register, class Key>
class SequentialFile{
private:
    struct registerInformation{
        AddressType position;
        char reference;
        Register record;
        registerInformation(){
            position = -1;
        }
        registerInformation(AddressType position_, char reference_, Register record_){
            position = position_;
            reference = reference_;
            record = record_;
        }
    };
    string dataFilePath;
    string auxFilePath;

    void writeRegister(int position, fstream& file, Register& record, FileID id){
        if(!file.is_open()) throw out_of_range("writeRegister() - Archivo no abierto");
        if(id == DATAFILE)
            file.seekp(position * sizeof(Register) + sizeof(int) + sizeof(char) + sizeof(bool), ios::beg);
        else if(id == AUXFILE)
            file.seekp(position * sizeof(Register), ios::beg);
        else
            throw invalid_argument("writeRegister() - id invalido");
        file.write((char*)& record, sizeof(Register));
    }

    void readRegister(int position, fstream& file, Register& record, FileID id){
        if(!file.is_open()) throw out_of_range("readRegister() - Archivo no abierto");
        if(id == DATAFILE)
            file.seekg(position * sizeof(Register) + sizeof(int) + sizeof(char) + sizeof(bool), ios::beg);
        else if(id == AUXFILE)
            file.seekg(position * sizeof(Register), ios::beg);
        else
            throw invalid_argument("readRegister() - id invalido");
        file.read((char*)& record, sizeof(Register));
    }

    int recordsNumber(fstream& file, FileID id){
        if(id != DATAFILE && id != AUXFILE)
            throw invalid_argument("recordsNumber() - id invalido");
        if(file.is_open()){
            file.seekg(0, ios::end);
            int n = (id == DATAFILE) ?
                    ((int) file.tellg() - sizeof(int) - sizeof(char) - sizeof(bool)) / sizeof(Register) :
                    (int) file.tellg() / sizeof(Register);
            file.seekg(0, ios::beg);
            return n;
        }return 0;
    }

    int recordsNumber(string filename, FileID id){
        fstream file(filename, ios::binary | ios::in);
        auto result = recordsNumber(file, id);
        file.close();
        return result;
    }

    void writeFirstRegisterInformation(fstream& file, AddressType positionOfTheFirstRecord, char referenceOfTheFirstRecord){
        if(!file.is_open()) throw out_of_range("writeFirstRegisterInformation() - Archivo no abierto");
        file.seekp(0, ios::beg);
        file.write((char *)& positionOfTheFirstRecord, sizeof(AddressType));
        file.write((char *)& referenceOfTheFirstRecord, sizeof(char));
    }

    void readFirstRegisterInformation(fstream& file, AddressType& positionOfTheFirstRecord, char& referenceOfTheFirstRecord){
        if(file.is_open()){
            file.seekg(0, ios::beg);
            file.read((char *)& positionOfTheFirstRecord, sizeof(AddressType));
            file.read((char *)& referenceOfTheFirstRecord, sizeof(char));
        }else{
            positionOfTheFirstRecord = -1;
            referenceOfTheFirstRecord = INVALID;
        }
    }

    void writeDeletedRegistersStatus(fstream& file, bool deletedRegisters){
        if(!file.is_open()) throw out_of_range("writeDeletedRegistersStatus() - Archivo no abierto");
        file.seekp(sizeof(AddressType)+sizeof(char), ios::beg);
        file.write((char *)& deletedRegisters, sizeof(bool));
    }

    void readDeletedRegistersStatus(fstream& file, bool& deletedRegisters){
        if(!file.is_open()) throw out_of_range("readDeletedRegistersStatus() - Archivo no abierto");
        file.seekg(sizeof(AddressType)+sizeof(char), ios::beg);
        file.read((char *)& deletedRegisters, sizeof(bool));
    }

    static bool compareRecords(Register& r1, Register& r2){
        return r1 < r2;
    }

    bool empty(FileID id){
        if(id == DATAFILE)
            return recordsNumber(this->dataFilePath, DATAFILE) == 0;
        else if(id == AUXFILE)
            return recordsNumber(this->auxFilePath, AUXFILE) == 0;
        else
            throw invalid_argument("readRegister() - id invalido");
    }

    bool possibleToUseBinarySearch(){
        fstream file(this->dataFilePath, ios::binary | ios::in);
        bool deletedRegisters;
        readDeletedRegistersStatus(file, deletedRegisters);
        file.close();
        if(empty(AUXFILE) && !deletedRegisters)
            return true;
        return false;
    }

    bool possibleToDeleteRegister(){
        int positionOfTheFirstRecord;
        char referenceOfTheFirstRecord;
        fstream dataFile(this->dataFilePath, ios::binary | ios::in);
        readFirstRegisterInformation(dataFile, positionOfTheFirstRecord, referenceOfTheFirstRecord);
        dataFile.close();
        return referenceOfTheFirstRecord != INVALID;
    }

public:
    SequentialFile(string dataFilePath, string auxFilePath){
        this->dataFilePath = dataFilePath;
        this->auxFilePath = auxFilePath;
    }

    void insertAll(vector<Register> &records){
        if(empty(DATAFILE)){
            sort(records.begin(), records.end(), compareRecords);
            fstream file(this->dataFilePath, ios::binary | ios::out);
            writeFirstRegisterInformation(file, 0, 'd');
            writeDeletedRegistersStatus(file, false);
            for (int i = 0; i < records.size(); i++){
                records[i].nextDel = i + 1;
                if(i == records.size() - 1) records[i].reference = INVALID;
                else records[i].reference = 'd';
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
            readRegister(mid, file, record, DATAFILE);
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
            readRegister(mid, file, record, DATAFILE);
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

    pair<registerInformation, registerInformation> sequentialSearch(Key key){
        fstream dataFile(this->dataFilePath, ios::binary | ios::in);
        fstream auxFile(this->auxFilePath, ios::binary | ios::in);
        int currentPosition, prevPosition = -1;
        char currentReference, prevReference = INVALID;
        readFirstRegisterInformation(dataFile, currentPosition, currentReference);
        Register currentRecord, prevRecord;
        while(currentReference != INVALID){
            if(currentReference == 'd') readRegister(currentPosition, dataFile,currentRecord, DATAFILE);
            else if(currentReference == 'a') readRegister(currentPosition, auxFile, currentRecord, AUXFILE);
            else throw invalid_argument("sequentialSearch() - reference invalido");
            if(!currentRecord.lessThanToKey(key))
                break;
            prevPosition = currentPosition;
            prevReference = currentReference;
            prevRecord = currentRecord;
            currentPosition = currentRecord.nextDel;
            currentReference = currentRecord.reference;
        }
        return {registerInformation(prevPosition, prevReference, prevRecord),
                  registerInformation(currentPosition, currentReference, currentRecord)};
    }

    vector<Register> search(Key key){
        if(possibleToUseBinarySearch())
            return usingBinarySearch(key);
        return usingSequentialSearch(key);
    }

    vector<Register> usingSequentialSearch(Key key){
        vector<Register> output;
        auto searchResult = sequentialSearch(key);
        Register currentRecord = searchResult.second.record;
        if(currentRecord.equalToKey(key)){
            output.push_back(currentRecord);
            fstream dataFile(this->dataFilePath, ios::binary | ios::in);
            fstream auxFile(this->auxFilePath, ios::binary | ios::in);
            int currentPosition = currentRecord.nextDel;
            char currentReference = currentRecord.reference;
            while(currentReference != INVALID){
                if(currentReference == 'd') readRegister(currentPosition, dataFile,currentRecord, DATAFILE);
                else if(currentReference == 'a') readRegister(currentPosition, auxFile, currentRecord, AUXFILE);
                else throw invalid_argument("usingSequentialSearch() - reference invalido");
                if(currentRecord.equalToKey(key)){
                    output.push_back(currentRecord);
                }
                currentPosition = currentRecord.nextDel;
                currentReference = currentRecord.reference;
            }
        }
        return output;
    }

    vector<Register> usingBinarySearch(Key key){
        fstream file(this->dataFilePath, ios::binary | ios::in);
        int position = bs_lower_bound(key);
        vector<Register> records;
        Register record;
        int recordsQuantity = recordsNumber(file, DATAFILE);
        while(position < recordsQuantity){
            readRegister(position, file, record, DATAFILE);
            if(record.equalToKey(key))
                records.push_back(record);
            else
                break;
            ++position;
        }
        return records;
    }

    vector<Register> searchPerRange(Key startKey, Key endKey){
        if(possibleToUseBinarySearch())
            return usingBinarySearchPerRange(startKey, endKey);
        return usingSequentialSearchPerRange(startKey, endKey);
    }

    vector<Register> usingSequentialSearchPerRange(Key startKey, Key endKey){
        vector<Register> output;
        auto searchResult = sequentialSearch(startKey);
        Register currentRecord = searchResult.second.record;
        int currentPosition = searchResult.second.position;
        char currentReference = searchResult.second.reference;
        fstream dataFile(this->dataFilePath, ios::binary | ios::in);
        fstream auxFile(this->auxFilePath, ios::binary | ios::in);
        while(currentReference != INVALID){
            if(currentRecord.greaterThanOrEqualToKey(startKey) && currentRecord.lessThanOrEqualToKey(endKey))
                output.push_back(currentRecord);
            currentPosition = currentRecord.nextDel;
            currentReference = currentRecord.reference;
            if(currentReference == 'd') readRegister(currentPosition, dataFile,currentRecord, DATAFILE);
            else if(currentReference == 'a') readRegister(currentPosition, auxFile, currentRecord, AUXFILE);
        }
        sort(output.begin(), output.end(), compareRecords);
        return output;
    }

    vector<Register> usingBinarySearchPerRange(Key startKey, Key endKey){
        fstream file(this->dataFilePath, ios::binary | ios::in);
        vector<Register> beginToEnd;
        int ptrB = bs_lower_bound(startKey);
        int ptrA = bs_upper_bound(endKey) - 1;
        Register record;
        while (ptrB <= ptrA){
            readRegister(ptrB, file, record, DATAFILE);
            beginToEnd.push_back(record);
            ptrB++;
        }
        file.close();
        return beginToEnd;
    }

    vector<Register> getSortedRecords(){
        fstream dataFile(this->dataFilePath, ios::binary | ios::in | ios::out);
        fstream auxFile(this->auxFilePath, ios::binary| ios::in | ios::out);
        int currentPosition, nextPosition, i = 0;
        char currentReference, nextReference;
        readFirstRegisterInformation(dataFile, currentPosition, currentReference);
        vector<Register> records;
        vector<Register> unordered_records;
        while (currentReference != INVALID){
            Register record;
            if(currentReference == 'd') readRegister(currentPosition, dataFile, record, DATAFILE);
            else if(currentReference == 'a') readRegister(currentPosition, auxFile, record, AUXFILE);
            else throw invalid_argument("getSortedRecords() - reference invalido");
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
            if(nextReference == 'd' || nextReference == INVALID){
                sort(unordered_records.begin(), unordered_records.end(), compareRecords);
                for(auto& r : unordered_records){
                    ++i;
                    r.nextDel = i;
                    r.reference = 'd';
                }
                records.insert(records.end(), unordered_records.begin(), unordered_records.end());
                unordered_records.clear();
            }
            currentReference = nextReference;
            currentPosition = nextPosition;
        }
        if(!unordered_records.empty()) cout << "NO ESTOY VACIO " << endl;
        if(!records.empty())
            records[records.size()-1].reference = INVALID;
        return records;
    }

    void reBuild(){
        auto sorted_records = getSortedRecords();
        remove(this->dataFilePath.c_str());
        remove(this->auxFilePath.c_str());
        fstream dataFile(this->dataFilePath, ios::binary | ios::out);
        fstream auxFile(this->auxFilePath, ios::binary | ios::out);
        writeFirstRegisterInformation(dataFile, 0, 'd');
        writeDeletedRegistersStatus(dataFile, false);
        for(int i = 0; i < sorted_records.size(); ++i)
            writeRegister(i, dataFile, sorted_records[i], DATAFILE);
        dataFile.close();
        auxFile.close();
    }

    void isFull(){
        int amountOfRecords = recordsNumber(this->auxFilePath, AUXFILE);
        if (amountOfRecords == CAPACITY)
            reBuild();
    }

    void add(Register record){
        if(empty(DATAFILE)){
            fstream file(this->dataFilePath, ios::binary | ios::out);
            record.nextDel = 1;
            record.reference = INVALID;
            writeFirstRegisterInformation(file, 0, 'd');
            writeDeletedRegistersStatus(file, false);
            writeRegister(0, file, record, DATAFILE);
            file.close();
            return;
        }
        isFull();
        fstream dataFile(this->dataFilePath, ios::binary | ios::in | ios::out);
        bool deletedRegisters;
        readDeletedRegistersStatus(dataFile, deletedRegisters);
        dataFile.close();

        if(deletedRegisters){
            addWithSequentialSearch(record);
        }else{
            addWithBinarySearch(record);
        }
    }

    void addWithBinarySearch(Register &record) {
        fstream auxFile(auxFilePath, ios::binary | ios::out | ios::app);
        fstream dataFile(dataFilePath, ios::binary | ios::in | ios::out);
        int positionOfTheFirstRecord;
        char referenceOfTheFirstRecord;
        readFirstRegisterInformation(dataFile, positionOfTheFirstRecord, referenceOfTheFirstRecord);
        int currentPosition = bs_upper_bound(record.getPrimaryKey()) - 1;
        if(currentPosition == -1){
            record.nextDel = positionOfTheFirstRecord;
            record.reference = referenceOfTheFirstRecord;
            writeFirstRegisterInformation(dataFile, recordsNumber(auxFilePath, AUXFILE), 'a');
        }else {
            Register r1;
            readRegister(currentPosition, dataFile, r1, DATAFILE);
            record.nextDel = r1.nextDel;
            record.reference = r1.reference;
            r1.nextDel = recordsNumber(auxFilePath, AUXFILE);
            r1.reference = 'a';
            writeRegister(currentPosition, dataFile, r1, DATAFILE);
        }
        auxFile.write((char*)& record, sizeof(Register));
        auxFile.close();
        dataFile.close();
    }

    void addWithSequentialSearch(Register &record) {
        fstream dataFile(dataFilePath, ios::binary | ios::in | ios::out);
        fstream auxFile(auxFilePath, ios::binary | ios::in | ios::out);
        int positionOfTheFirstRecord;
        char referenceOfTheFirstRecord;
        readFirstRegisterInformation(dataFile, positionOfTheFirstRecord, referenceOfTheFirstRecord);
        auto searchResult = sequentialSearch(record.getPrimaryKey());
        int currentPosition = searchResult.second.position;
        char currentReference = searchResult.second.reference;
        char prevReference = searchResult.first.reference;

        if(prevReference == INVALID)
            writeFirstRegisterInformation(dataFile, recordsNumber(auxFilePath, AUXFILE), 'a');

        if(currentReference == INVALID && prevReference == INVALID){
            record.nextDel = recordsNumber(dataFile, DATAFILE);
            record.reference = INVALID;
        }else if(prevReference == INVALID){
            record.nextDel = positionOfTheFirstRecord;
            record.reference = referenceOfTheFirstRecord;
        }else{
            Register currentRecord = searchResult.second.record;
            record.nextDel = currentRecord.nextDel;
            record.reference = currentRecord.reference;
            currentRecord.nextDel = recordsNumber(auxFilePath, AUXFILE);
            currentRecord.reference = 'a';
            if(currentReference == INVALID) {
                int prevPosition = searchResult.first.position;
                currentPosition = prevPosition;
                currentReference = prevReference;
            }
            if(currentReference == 'd') writeRegister(currentPosition, dataFile, currentRecord, DATAFILE);
            else if(currentReference == 'a') writeRegister(currentPosition, auxFile, currentRecord, AUXFILE);
            else throw invalid_argument("removeWithSequentialSearch() - reference invalido");
        }
        auxFile.close();
        auxFile.open(auxFilePath, ios::binary | ios::out | ios::app);
        auxFile.write((char*)& record, sizeof(Register));
        auxFile.close();
        dataFile.close();
    }

    void removeRecord(Key keyToRemove){
        if(!possibleToDeleteRegister()) return ;
        auto searchResult = sequentialSearch(keyToRemove);
        Register currentRecord = searchResult.second.record;
        if(!currentRecord.equalToKey(keyToRemove)) return ;
        fstream auxFile(this->auxFilePath, ios::binary | ios::in | ios::out);
        fstream dataFile(this->dataFilePath, ios::binary | ios::in | ios::out);

        int prevPosition = searchResult.first.position;
        char prevReference = searchResult.first.reference;
        Register prevRecord = searchResult.first.record;

        int currentPosition = searchResult.second.position;
        char currentReference = searchResult.second.reference;

        int nextPosition = currentRecord.nextDel;
        char nextReference = currentRecord.reference;

        if(prevReference == INVALID){
            writeFirstRegisterInformation(dataFile, nextPosition, nextReference);
        }else{
            prevRecord.nextDel = nextPosition;
            prevRecord.reference = nextReference;
            if(prevReference == 'd') writeRegister(prevPosition, dataFile, prevRecord, DATAFILE);
            else if(prevReference == 'a') writeRegister(prevPosition, auxFile, prevRecord, AUXFILE);
            else throw invalid_argument("remove() - reference invalido");
        }
        currentRecord.nextDel = -1;
        currentRecord.reference = INVALID;
        if(currentReference == 'd') writeRegister(currentPosition, dataFile, currentRecord, DATAFILE);
        else if(currentReference == 'a') writeRegister(currentPosition, auxFile, currentRecord, AUXFILE);
        else throw invalid_argument("remove() - reference invalido");

        writeDeletedRegistersStatus(dataFile, true);
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
            ++i;
            if(r.nextDel == -1) continue;
            cout << "******************************" << endl;
            r.showData();
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "Aux file: " << endl;
        i = 0;
        while (i < recordsNumberAuxfile){
            readRegister(i, auxFile, r, AUXFILE);
            ++i;
            if(r.nextDel == -1) continue;
            cout << "******************************" << endl;
            r.showData();
        }
        cout << ">>>>>>>>>>>>>>>>>>>>>>>" << endl;
        dataFile.close();
        auxFile.close();
    }
};

#endif // SEQUENTIALFILE_H
