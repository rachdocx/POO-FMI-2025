#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Customer {
    private:
        string name;
        string phone_number;
        string adress;
        string CNP;
    public:
        Customer(string name = "N/A", string phone_number = "N/A", string adress = "N/A", string CNP = "N/A") {
            this->adress = adress;
            this->name = name;
            this->phone_number = phone_number;
            this ->CNP = CNP;

        }
        ~Customer() = default;
    friend ostream & operator << (ostream & os, Customer & obj) {
        os << obj.name << " " << obj.phone_number << " " << obj.adress <<" "<<obj.CNP<<endl;
        return os;
    }
};
class PachetClasic {
    protected:
        int no_of_meals;
        bool alone;
        string alergeni;
    public:
        PachetClasic(int no_of_meals =0, bool alone = false, string alergeni = "N/A") {
            this->no_of_meals = no_of_meals;
            this -> alone = alone;
            this -> alergeni = alergeni;
        }
        ~PachetClasic() = default;
    friend ostream & operator << (ostream & os, PachetClasic & obj) {
        os << obj.no_of_meals << " " << obj.alone << " " << obj.alergeni<<endl;
        return os;
    }
};
class PachetSportiv: virtual public PachetClasic {
protected:
    int no_of_walks;
public:
    PachetSportiv(int no_of_meals =0, bool alone = false, string alergeni = "N/A", int no_of_walks = 0): PachetClasic(no_of_meals,alone,alergeni) {
        this->no_of_walks = no_of_walks;
    }
    ~PachetSportiv() = default;
    friend ostream & operator << (ostream & os, PachetSportiv & obj) {
        os << (PachetClasic&)(obj)<<" "<<obj.no_of_walks<<endl;
        return os;
    }
};
class PachetConfort: virtual public PachetClasic {
protected:
    vector <string> activity;
    int no_of_activities;
public:
    PachetConfort(int no_of_meals =0, bool alone = false, string alergeni = "N/A", vector<string> activity= {}, int no_of_activities = 2): PachetClasic(no_of_meals,alone,alergeni) {
        this->activity = activity;
        this->no_of_activities = no_of_activities;
    }
    ~PachetConfort() = default;
    friend ostream & operator << (ostream & os, PachetConfort obj) {
        os << (PachetClasic&)(obj)<<" "<<obj.no_of_activities<<endl;
        for (int i=0;i<obj.no_of_activities;++i)
            os<<obj.activity[i]<<" ";
        return os;
    }
};
class PachetVIP: public PachetSportiv, public PachetConfort {
private:
    vector<string> hours_of_walks;
    vector<string> durata_plimbare;
public:
    PachetVIP(int no_of_meals =0, bool alone = false, string alergeni = "N/A", vector<string> activity= {}, int no_of_activities = 2, int no_of_walks = 0, vector<string>hours_of_walks={}, vector<string>durata_plimbare ={}):PachetConfort(no_of_meals,alone,alergeni,activity,no_of_activities),PachetSportiv(no_of_meals,alone,alergeni,no_of_walks) {
        this->no_of_walks = no_of_walks;
        this->durata_plimbare=durata_plimbare;
    }
    ~PachetVIP() = default;

};
class Animal {
private:
    int age;
    string race;
    const int id_animal;

    static int id_generator;
public:
    Animal(int age =0, string race ="N/A"):id_animal(id_generator++) {
        this->age = age;
        this->race = race;

    }
    ~Animal() = default;
    const int getID() {
        return id_animal;
    }
    friend ostream & operator << (ostream & os, Animal & obj) {
        os << obj.age << " " << obj.race << " " << obj.id_animal <<endl;
        return os;
    }
};
class Formular {
private:
    string data_inc;
    string data_fin;
    PachetClasic * pachet;
    Customer * client;
    Animal * animal1;
    int pret;
    int cod;
public:
    Formular(string data_inc ="N/A", string data_fin="N/A", PachetClasic * pachet = nullptr, int pret =0, int cod =0, Customer * client = nullptr, Animal * animal1 = nullptr) {
        this->data_inc=data_inc;
        this->data_fin = data_fin;
        this->pachet = pachet;
        this ->client = client;
        this -> animal1 = animal1;
        this ->pret = pret;
        this -> cod = cod;


    }
    ~Formular() {
        if (pachet != nullptr)
            delete pachet;
        if (animal1 != nullptr)
            delete animal1;
        if (client != nullptr)
            delete client;
    }
    friend ostream & operator << (ostream & os, Formular & obj) {
        os << obj.data_inc << " " << obj.cod << " " << obj.pret <<endl;
        return os;
    }
    const int getCod() {
        return cod;
    }
    Animal * getAnimal() {
        return animal1;
    }
    Customer * getCustomer() {
        return client;
    }
    PachetClasic * getPachet() {
        return pachet;
    }
};
int Animal::id_generator =0;
int op;
const int pret_baza = 100;
string g_data_inc, g_data_fin,g_alergeni,g_pachet_ales,g_race, g_name, g_phone, g_adress, g_CNP;
bool g_alone;
int g_no_of_meals,g_age,g_no_activity,g_no_walks;
vector<string>g_activity;
vector<string>g_hours;
vector<string>g_durata;
int main() {
    vector <Formular *> formulare;
    op =-1;
    do {
        cout << "1. Adaugati un formular\n";
        cout << "2. Afisati Formulare\n";
        cout << "3. Afisati un formular specific\n";
        cout << "4. Afisati formularele ordonate crescator\n";
        cin >> op;
        switch (op) {
            case 1: {
                cout<<"Introduceti data inceput, data final si pachetul ales\n";
                cin>>g_data_inc>>g_data_fin>>g_pachet_ales;
                if (g_pachet_ales == "PachetClasic") {
                    int pret = pret_baza;
                    cout<<"Introduceti numarul de mese, alergeni, si bool value daca e singur sau nu";
                    cin>>g_no_of_meals>>g_alergeni>>g_alone;
                    PachetClasic * temp = new PachetClasic(g_no_of_meals, g_alone,g_alergeni);
                    cout<<"Introduceti varsta si rasa animalului";
                    cin>>g_age>>g_race;
                    Animal * temp1 = new Animal(g_age, g_race);
                    cout<<"Introduceti numele, adresa, telefonul si CNP ul clientului";
                    cin>>g_name>>g_adress>>g_phone>>g_CNP;
                    Customer * temp2 = new Customer(g_name,g_phone,g_adress,g_CNP);
                    if (g_alone == true)
                        pret = pret + 30;
                    Formular * temp3 = new Formular(g_data_inc,g_data_fin,temp,pret,temp1->getID(),temp2,temp1);
                    formulare.push_back(temp3);
                }
                if (g_pachet_ales == "PachetConfort") {
                    int pret = pret_baza;
                    cout<<"Introduceti numarul de mese, alergeni, si bool value daca e singur sau nu si nr de activitiati";
                    cin>>g_no_of_meals>>g_alergeni>>g_alone>>g_no_activity;
                    cout<<"Introduceti activitatile";
                    for (int i=0;i<g_no_activity;++i) {
                        string temp4;
                        cin>>temp4;
                        g_activity.push_back(temp4);
                    }
                    PachetConfort * temp = new PachetConfort(g_no_of_meals, g_alone,g_alergeni,g_activity,g_no_activity);
                    cout<<"Introduceti varsta si rasa animalului";
                    cin>>g_age>>g_race;
                    Animal * temp1 = new Animal(g_age, g_race);
                    cout<<"Introduceti numele, adresa, telefonul si CNP ul clientului";
                    cin>>g_name>>g_adress>>g_phone>>g_CNP;
                    Customer * temp2 = new Customer(g_name,g_phone,g_adress,g_CNP);
                    if (g_alone == true)
                        pret = pret + 30;
                    pret = pret + 50 * g_no_activity;
                    Formular * temp3 = new Formular(g_data_inc,g_data_fin,temp,pret,temp1->getID(),temp2,temp1);
                    formulare.push_back(temp3);
                }
                if (g_pachet_ales == "PachetSportiv") {
                    int pret = pret_baza;
                    cout<<"Introduceti numarul de mese, alergeni, si bool value daca e singur sau nu";
                    cin>>g_no_of_meals>>g_alergeni>>g_alone>>g_no_walks;
                    PachetSportiv * temp = new PachetSportiv(g_no_of_meals, g_alone,g_alergeni, g_no_walks);
                    cout<<"Introduceti varsta si rasa animalului";
                    cin>>g_age>>g_race;
                    Animal * temp1 = new Animal(g_age, g_race);
                    cout<<"Introduceti numele, adresa, telefonul si CNP ul clientului";
                    cin>>g_name>>g_adress>>g_phone>>g_CNP;
                    Customer * temp2 = new Customer(g_name,g_phone,g_adress,g_CNP);
                    if (g_alone == true)
                        pret = pret + 30;
                    pret = pret + 30 * g_no_walks;
                    Formular * temp3 = new Formular(g_data_inc,g_data_fin,temp,pret,temp1->getID(),temp2,temp1);
                    formulare.push_back(temp3);
                }
                if (g_pachet_ales == "PachetVIP") {
                    int pret = pret_baza;
                    cout<<"Introduceti numarul de mese, alergeni, si bool value daca e singur sau nu, numarul de activitati si numarul de plimbari";
                    cin>>g_no_of_meals>>g_alergeni>>g_alone>>g_no_activity>>g_no_walks;
                    cout <<"Introduceti activitatile";
                    for (int i=0;i<g_no_activity;++i) {
                        string temp4;
                        cin>>temp4;
                        g_activity.push_back(temp4);
                    }
                    cout<<"Introfuceti ora si durata plimbarilor";
                    for (int i =0; i<g_no_walks;++i) {
                        string temp4, temp5;
                        cin>>temp4>>temp5;
                        g_hours.push_back(temp4);
                        g_durata.push_back(temp5);
                    }
                    PachetVIP * temp = new PachetVIP(g_no_of_meals, g_alone,g_alergeni,g_activity,g_no_activity,g_no_walks,g_hours,g_durata);
                    cout<<"Introduceti varsta si rasa animalului";
                    cin>>g_age>>g_race;
                    Animal * temp1 = new Animal(g_age, g_race);
                    cout<<"Introduceti numele, adresa, telefonul si CNP ul clientului";
                    cin>>g_name>>g_adress>>g_phone>>g_CNP;
                    Customer * temp2 = new Customer(g_name,g_phone,g_adress,g_CNP);
                    if (g_alone == true)
                        pret = pret + 30;
                    pret = pret + 50 * g_no_activity;
                    Formular * temp3 = new Formular(g_data_inc,g_data_fin,temp,pret,temp1->getID(),temp2,temp1);
                    formulare.push_back(temp3);
                }
                break;
            }
            case 2: {
                for(auto i:formulare) {
                    cout<<*i;
                }
                break;
            }
            case 3: {
                cout<<"Introduceti cod formular\n";
                int cod;
                cin>>cod;
                for (auto i: formulare) {
                    if (cod == i->getCod()) {
                        cout<<"Oferta, Client sau Animal?";
                        string optiune;
                        cin >> optiune;
                        if (optiune == "Client") {
                            cout<<*(i->getCustomer());
                        }
                        if (optiune == "Animal") {
                            cout<<*(i->getAnimal());
                        }
                        if (optiune == "Oferta") {
                            cout<<*(i->getPachet());
                        }
                    }
                }
                break;
            }


        }
    }while (op!=0);
    return 0;
}
