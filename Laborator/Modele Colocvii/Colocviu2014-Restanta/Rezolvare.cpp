#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Angajat {
    protected:
        string first_name;
        string last_name;
        string contract_date;
        int prima;
    public:
        Angajat(string first_name="N/A", string last_name="N/A", string contract_date="N/A", int prima=0) {
            this->first_name=first_name;
            this->last_name=last_name;
            this->contract_date=contract_date;
            this->prima=prima;
        }
        ~Angajat() {

        }
        friend istream & operator >> (istream & is, Angajat & obj) {
            is >> obj.first_name >> obj.last_name >> obj.contract_date >> obj.prima;
            return is;
        }
        friend ostream & operator << (ostream & os, Angajat & obj) {
            os << obj.first_name << " " << obj.last_name << " " << obj.contract_date << " " <<  obj.prima;
            return os;
        }
        virtual const string getType() const = 0;
        virtual const string getDepartmentType() const = 0;


};
class AngajatCuOra:public Angajat {
    string contract_ending;
    public:
        AngajatCuOra(string first_name="N/A", string last_name="N/A", string contract_date="N/A", string contract_ending="N/A", int prima=0):Angajat(first_name,last_name, contract_date, prima) {
            this->contract_ending=contract_ending;
            if (stoi(contract_date.substr(6)) <= 2014)
                this->prima = this->prima/2;

        }
        ~AngajatCuOra() {}
        friend istream & operator >> (istream & is, AngajatCuOra & obj) {
            is >> static_cast<Angajat&>(obj);
            is >> obj.contract_ending;
            return is;
        }
        friend ostream & operator << (ostream & os, AngajatCuOra & obj) {
            // os << static_cast<Angajat&>(obj);
            // os << " " << obj.contract_ending;
            os << obj.first_name <<" "<< obj.last_name<<" "<< obj.getType() << " "<< obj.contract_date << " "<<obj.contract_ending<<" " <<obj.prima << endl;
            return os;
        }
        const string getType() const {
            return "plata_cu_ora";
        }
        const string getDepartmentType() const {
            return "N/A";
        }
        const string getContractEnd() const {
            return contract_ending;
        }
};
class AngajatPermanentTesa:public Angajat {
    private:
        string weekend;
    public:
    AngajatPermanentTesa(string first_name="N/A", string last_name="N/A", string contract_date="N/A", string weekend="N/A", int prima = 0): Angajat(first_name, last_name, contract_date, prima) {
        this->weekend=weekend;
    };
    ~AngajatPermanentTesa() {}
    const string getType()const {
        return "contract_permanent";
    }
    const string getDepartmentType() const{
        return "TESA";
    }
    friend istream & operator >> (istream & is, AngajatPermanentTesa & obj) {
        is >> static_cast<Angajat&>(obj);
        is >> obj.weekend;
        return is;
    }
    friend ostream & operator << (ostream & os, AngajatPermanentTesa & obj) {
        // os << static_cast<Angajat&>(obj);
        // os << " " << obj.weekend;
        os << obj.first_name <<" "<< obj.last_name<<" "<< obj.getType() << " "<< obj.contract_date <<" " <<obj.getDepartmentType() <<" "<< obj.weekend<<" " <<obj.prima << endl;
        return os;
    }
    string const getWeekend() {
        return weekend;
    }
};
class AngajatPermanentLucrativ:public Angajat {
    private:
        int nr_minori;
    public:
    AngajatPermanentLucrativ(string first_name="N/A", string last_name="N/A", string contract_date="N/A", int nr_minori = 0, int prima = 0):Angajat(first_name, last_name, contract_date, prima) {
        int val = 2014-stoi(this->contract_date.substr(6));
        this->prima = ((val * this->prima)/100) * nr_minori + this->prima;
        this->nr_minori=nr_minori;
    }
    ~AngajatPermanentLucrativ(){};
    const string getType() const{
        return "contract_permanent";
    }
    const string getDepartmentType()const  {
        return "lucrativ";
    }
    friend istream & operator >> (istream & is, AngajatPermanentLucrativ & obj) {
        is >> static_cast<Angajat&>(obj);
        is >> obj.nr_minori;
        return is;
    }
    friend ostream & operator << (ostream & os, AngajatPermanentLucrativ & obj) {
        // os << static_cast<Angajat&>(obj);
        // os << " " << obj.nr_minori;
        os << obj.first_name <<" "<< obj.last_name<<" "<< obj.getType() << " "<< obj.contract_date <<" " <<obj.getDepartmentType() <<" "<< obj.nr_minori<<" " <<obj.prima << endl;
        return os;
    }
    int const getNrMinori() {
        return nr_minori;
    }
};
int op = -1;
const int global_prima = 400;
#include <fstream>
ifstream f("input.txt");
string global_first_name, global_last_name, global_contract_type, global_hire_date, global_contract_end, global_department, global_weekend;
int nr_minori;
int main() {
    vector <Angajat *> angajati;
    //de facut meniu d ala singletone daca ai chef.
    while (f>>global_first_name>>global_last_name) {
        f>>global_contract_type;
        if (global_contract_type == "plata_cu_ora") {
            f>>global_hire_date>>global_contract_end;
            angajati.push_back(new AngajatCuOra(global_first_name,global_last_name,global_hire_date,global_contract_end,global_prima));
        }
        if (global_contract_type == "contract_permanent") {
            f>>global_hire_date>>global_department;
            if (global_department == "TESA") {
                f>>global_weekend;
                angajati.push_back(new AngajatPermanentTesa(global_first_name,global_last_name,global_hire_date,global_weekend,global_prima));
            }
            else if (global_department == "lucrativ") {
                f >> nr_minori;
                angajati.push_back(new AngajatPermanentLucrativ(global_first_name,global_last_name,global_hire_date,nr_minori,global_prima));
            }
        }
    }
    do {
        cout << "\n1. Afisati toti angajatii";
        cout << "\n2. Afisati toti angajatii lucrativi permanenti care au copii minori in ingrijire";
        cout << "\n3. Afisati angajatii cu rezervare la munte in al doilea weekend din decembrie";
        cout << "\n4. Afisati angajatii in regim de plata cu ora, care au contract pana la cel putin martie 2015\n";
        cin >> op;
        try {
            if (op < 0 || op > 4)
                throw runtime_error ("Invalid Option");
            switch (op) {
                case 1: {
                    for (auto i: angajati) {
                        if (i->getType() == "plata_cu_ora") {
                            AngajatCuOra * temp = dynamic_cast<AngajatCuOra*>(i);
                            cout<< *temp << "\n";
                        }
                        if (i->getType() == "contract_permanent") {
                            if (i->getDepartmentType() == "TESA") {
                                AngajatPermanentTesa * temp = dynamic_cast<AngajatPermanentTesa *>(i);
                                cout << * temp << "\n";
                            }
                            if (i->getDepartmentType() == "lucrativ") {
                                AngajatPermanentLucrativ * temp = dynamic_cast<AngajatPermanentLucrativ*>(i);
                                cout << * temp << "\n";
                            }
                        }
                    }

                    break;
                }
                case 2: {
                    for (auto i:angajati) {
                        if (i->getDepartmentType() == "lucrativ") {
                            AngajatPermanentLucrativ * temp = dynamic_cast<AngajatPermanentLucrativ*>(i);
                            if (temp->getNrMinori()>0)
                                cout<<*temp;
                        }
                    }

                    break;
                }
                case 3: {
                    for (auto i:angajati) {
                        if (i->getDepartmentType() == "TESA") {
                            AngajatPermanentTesa * temp = dynamic_cast<AngajatPermanentTesa*>(i);
                            string temp1 = temp->getWeekend();
                            if (temp1[temp1.size()-1]=='2')
                                cout<<*temp;
                        }
                    }
                    break;
                }
                case 4: {
                    for (auto i: angajati) {
                        AngajatCuOra * temp;
                        if (temp = dynamic_cast<AngajatCuOra*>(i)) {
                            string temp1;
                            temp1 = temp->getContractEnd();
                            if (stoi(temp1.substr(6)) > 2015)
                                cout << *temp;
                            else
                                if (stoi(temp1.substr(6)) ==  2015 && stoi(temp1.substr(4,5)) > 3)
                                    cout << *temp;
                        }
                    }
                    break;
                }
            }
        }catch (const runtime_error & e) {
            cout<<e.what()<<endl;
        }
    }while (op!=0);
    return 0;
}
