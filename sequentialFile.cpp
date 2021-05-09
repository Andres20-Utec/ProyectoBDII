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
        if(numRecords(this->auxfile) > 0) reBuild(); //Evitar el salto de archivos
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

    vector<Record> search_in_range(string start, string end)
    { 
        if(numRecords(this->auxfile) > 0) reBuild(); //Evitar el salto de archivos
        if(start.size() > 20) start = start.substr(0, 20);
        if(end.size() > 20) end = end.substr(0, 20);
        vector<Record> beginToEnd;
        int ptrB = bs_lower_bound(start);
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
        fstream fsAux(this->auxfile, ios::binary | ios::out | ios::app);
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