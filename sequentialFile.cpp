#include "record.cpp"
#include <algorithm>
#define CAPACITY 5

enum IDFile {AUXFILE, DATAFILE};

class SequentialFile
{
private:
    string datafile;
    string auxfile;
    bool empty;
    int amount_control;

    static void sf_write(int pos, fstream& file, Record& r1, IDFile id){
        if(id == DATAFILE)
            file.seekp(pos*sizeof(Record) + sizeof(int) + sizeof(char), ios::beg);
        else
            file.seekp(pos*sizeof(Record), ios::beg);
        file << r1;
    }

    static void sf_read(int pos, fstream&file, Record& r1, IDFile id){
        if(id == DATAFILE)
            file.seekg(pos*sizeof(Record) + sizeof(int) + sizeof(char), ios::beg);
        else
            file.seekg(pos*sizeof(Record), ios::beg);
        file >> r1;
    }

    static int numRecords(fstream& file, IDFile id){
        if(file.is_open()) {
            int prev = file.tellp();
            file.seekp(0, ios::end);
            int n = (id == DATAFILE) ? ((int) file.tellp() - sizeof(int) - sizeof(char)) / sizeof(Record) :
                    (int) file.tellp() / sizeof(Record);
            file.seekp(prev, ios::beg);
            return n;
        }return 0;
    }

    static int numRecords(string filename, IDFile id){
        ifstream file(filename, ios::binary);
        if(file.is_open()){
            file.seekg(0, ios::end);
            int n = (id == DATAFILE) ? ((int) file.tellg() - sizeof(int) - sizeof(char)) / sizeof(Record) :
                    (int) file.tellg() / sizeof(Record);
            file.close();
            return n;
        }
        else return 0;
    }

    static void initialize_first_values(fstream& file, int first_position, char first_reference){
        file.seekp(0, ios::beg);
        file.write((char *)&first_position, sizeof(int));
        file.write((char *)&first_reference, sizeof(char));
    }

public:
    SequentialFile(string datafile, string auxfile){
        this->datafile = datafile;
        this->auxfile = auxfile;
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