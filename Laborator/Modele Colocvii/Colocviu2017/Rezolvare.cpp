#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Fisa {
protected:
    string adresa;
    int suprafata;
    int chirie;
    bool sold, rent;
public:
    int getSuprafata() {
        return suprafata;
    }
    int getChirie() {
        return chirie;
    }
    Fisa(string adresa="N/A", int suprafata =0, int chirie =0, bool sold = false, bool rent = false){
        this->adresa=adresa;
        this->suprafata=suprafata;
        this->chirie=chirie;
        this->rent=rent;
        this->sold=sold;
    }
    virtual ~Fisa() = default;
    friend ostream & operator << (ostream & os,   Fisa & obj) {
        os<<"Adresa este: "<<obj.adresa;
        os<<"\nSuprafata este: "<<obj.suprafata;
        os<<"\nChiria este: "<<obj.chirie;
        if (obj.rent)
            os<<"\nProprietatea este data in chirie";
        else
            os<<"\nProprietatea nu este data in chirie";
        if (obj.sold)
            os<<"\nProprietatea este vanduta";
        else
            os<<"\nProprietatea nu este vanduta";

        return os;
    }
    friend istream & operator >> (istream & is, Fisa & obj) {
        cout<<"Introduceti adresa: ";
        is>>obj.adresa;
        cout<<"Introduceti suprafata: ";
        is>>obj.suprafata;
        cout<<"Introduceti chiria: ";
        is>>obj.chirie;
        cout<<"Este proprietatea in chirie? ";
        is>>obj.rent;
        cout<<"Este proprietatea vadnuta? ";
        is>>obj.sold;
        return is;
    }
    void setSold() {
        this->sold=true;
    }
    void setRent() {
        this->rent=true;
    }
    void setNotSold() {
        this->sold=false;
    }
    void setNotRent() {
        this->rent=false;
    }
    bool isSold() {
        return this->sold;
    }
    bool isRent() {
        return  this->rent;
    }
    // virtual int getChirie() const =0;
};
class FisaApartament:public Fisa {
private:
    int nr_etaj,nr_camere;
public:

    FisaApartament(string adresa="N/A", int suprafata =0, int chirie =0, bool rent = false, bool sold = false, int nr_etaj = 0, int nr_camere =0): Fisa(adresa,suprafata,chirie,rent,sold) {
        this->nr_etaj=nr_etaj;
        this->nr_camere=nr_camere;
    }
    ~FisaApartament() = default;
    friend ostream & operator << (ostream & os,   FisaApartament & obj) {
        os<<static_cast<Fisa&>(obj);
        os<<"\nApartamentul e la etajul: "<<obj.nr_etaj;
        os<<"\nApartamentul are: "<<obj.nr_camere<<" camere.";
        return os;
    }
    friend istream & operator >> (istream & is,   FisaApartament & obj) {
        is>>static_cast<Fisa&>(obj);
        cout<<"Introduceti etajul: ";
        is>>obj.nr_etaj;
        cout<<"Introduceti nr de camere: ";
        is>>obj.nr_camere;
    }
};
class FisaCasa:public Fisa {
private:
    int nr_niveluri, suprafata_curte;
public:
    int suprafataCurte() {
        return suprafata_curte;
    }
    FisaCasa(string adresa="N/A", int suprafata =0, int chirie =0, bool rent = false, bool sold = false, int nr_niveluri = 0, int suprafata_curte =0) :Fisa(adresa,suprafata,chirie,rent,sold) {
        this->nr_niveluri=nr_niveluri;
        this->suprafata_curte=suprafata_curte;
    }
    ~FisaCasa() = default;
    friend istream & operator >> (istream & is,   FisaCasa & obj) {
        is>>static_cast<Fisa&>(obj);
        cout<<"Introduceti numarul de niveluri: ";
        is>>obj.nr_niveluri;
        cout<<"Introduceti suprafata curtii: ";
        is>>obj.suprafata_curte;
        return is;
    }
    friend ostream & operator << (ostream & os,   FisaCasa & obj) {
        os<<static_cast<Fisa&>(obj);
        os<<"\nCasa are "<<obj.nr_niveluri<<" niveluri.";
        os<<"\nSuprafata curtii este de: "<<obj.suprafata_curte;
        return os;
    }
};
class Contract {
protected:
    string nume;
    string data_inc,data_fin;
    float chirie_lunara;
    Fisa * proprietate;
public:
    Contract(string nume = "N/A", string data_inc= "N/A", string data_fin= "N/A", Fisa * proprietate = nullptr) {
        this->nume=nume;
        this->data_fin=data_fin;
        this->data_inc=data_inc;
        this->chirie_lunara=0;
        this->proprietate=proprietate;
    }
    virtual ~Contract() {
        if (proprietate != nullptr) {
            delete proprietate;
        }
    }
    Fisa * getProprietate() {
        return proprietate;
    }
    string getNume() {
        return this->nume;
    }
};
class ContractChirie:public Contract {
private:
public:
    ContractChirie(string nume = "N/A", string data_inc= "N/A", string data_fin= "N/A", Fisa * proprietate = nullptr):Contract(nume,data_inc,data_fin, proprietate) {
        if (FisaApartament * temp = dynamic_cast<FisaApartament *>(this->getProprietate())) {
            if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>2) {
                //01/04/2025
                this->chirie_lunara=temp->getChirie()*temp->getSuprafata()*0.9;
            }else
            if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>1) {
                //01/04/2025
                this->chirie_lunara=temp->getChirie()*temp->getSuprafata()*0.95;
            }
        }else if (FisaCasa * temp = dynamic_cast<FisaCasa *>(this->getProprietate())) {
            if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>2) {
                //01/04/2025
                this->chirie_lunara=temp->getChirie()*(temp->getSuprafata()-0.2*temp->suprafataCurte())*0.9;
            }else
                if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>1) {
                    //01/04/2025
                    this->chirie_lunara=temp->getChirie()*(temp->getSuprafata()-0.2*temp->suprafataCurte())*0.95;
                }
        }

    }
    ~ContractChirie() = default;
};
class ContractCumparare:public Contract {
private:
public:
    ContractCumparare(string nume = "N/A", string data_inc= "N/A", string data_fin= "N/A", Fisa * proprietate = nullptr):Contract(nume,data_inc,data_fin, proprietate) {
        float discount = 0;
        if (this->data_fin ==  this->data_inc)
            discount=0.9;
        else if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>5) {
            //5 ani = 60 luni
            discount = 0.5;
        }
        else if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>10) {
            discount = 0.7;
        }
        else if (stoi(this->data_fin.substr(6))-stoi(this->data_inc.substr(6))>20) {
            discount =-1;
        }
        try {
            if (discount = -1)
                throw runtime_error("Nu oferim credite de peste 24 luni");
            if (FisaApartament * temp = dynamic_cast<FisaApartament *>(this->getProprietate())) {
                this->chirie_lunara=240*temp->getSuprafata()*discount;
            }else if (FisaCasa * temp = dynamic_cast<FisaCasa *>(this->getProprietate())) {
                this->chirie_lunara=240*(temp->getSuprafata()*0.2*temp->suprafataCurte())*discount;
            }
        }catch (runtime_error & e) {
            cout<<e.what();
        }
    }
    ~ContractCumparare() = default;
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    inline static Menu * MENU;
    vector<Fisa*> proprietati;
    vector <Contract*> contracte;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    void adaugareProprietate(){
        string tip;
        cout<<"Introduceti tipul de proprietate: ";
        cin>>tip;
        if (tip=="Apartament") {
            FisaApartament * temp = new FisaApartament();
            cin>>*temp;
            proprietati.push_back(temp);
        }
        else if (tip == "Casa") {
            FisaCasa * temp = new FisaCasa();
            cin>>*temp;
            proprietati.push_back(temp);
        }
    }
    void adaugareContract() {
        string tip_cont;
        string global_nume, global_datai,global_dataf;
        cout<<"Introduceti tipul de contract: ";
        cin>>tip_cont;
        if (tip_cont == "Chirie") {
            string tip;
            cout<<"Introduceti tipul de proprietate: ";
            cin>>tip;
            if(tip=="Apartament") {
                FisaApartament * temp = new FisaApartament();
                cin>>*temp;
                try {
                    if (temp->isSold())
                        throw runtime_error("Proprietatea este deja vanduta!");
                    if (temp->isRent())
                        throw runtime_error("Proprietatea este deja in chirie!");
                    proprietati.push_back(temp);
                    cout<<"Introduceti numele contractului: ";
                    cin>>global_nume;
                    cout<<"Introduceti data finalizarii contractului: ";
                    cin>>global_dataf;
                    cout<<"Introduceti data inceperii contractului: ";
                    temp->setSold();
                    ContractChirie * temp1 =  new ContractChirie(global_nume,global_datai,global_dataf,temp);
                    contracte.push_back(temp1);
                }catch (runtime_error & e) {
                    cout<<e.what();
                }
            }
            else if (tip == "Casa") {
                FisaCasa * temp = new FisaCasa();
                cin>>*temp;
                try {
                    if (temp->isSold())
                        throw runtime_error("Proprietatea e deja vanduta!");
                    if (temp->isRent())
                        throw runtime_error("Proprietatea este deja in chirie!");
                    proprietati.push_back(temp);
                    cout<<"Introduceti numele contractului: ";
                    cin>>global_nume;
                    cout<<"Introduceti data finalizarii contractului: ";
                    cin>>global_dataf;
                    cout<<"Introduceti data inceperii contractului: ";
                    temp->setSold();
                    ContractChirie * temp1 =  new ContractChirie(global_nume,global_datai,global_dataf,temp);
                    contracte.push_back(temp1);
                }catch (runtime_error & e) {
                    cout<<e.what();
                }
            }
        }
        else if (tip_cont == "Cumparare") {
            string tip;
            cout<<"Introduceti tipul de proprietate: ";
            cin>>tip;
            if(tip=="Apartament") {
                FisaApartament * temp = new FisaApartament();
                cin>>*temp;
                try {
                    if (temp->isSold())
                        throw runtime_error("Proprietatea este deja vanduta!");
                    proprietati.push_back(temp);
                    cout<<"Introduceti numele contractului: ";
                    cin>>global_nume;
                    cout<<"Introduceti data finalizarii contractului: ";
                    cin>>global_dataf;
                    cout<<"Introduceti data inceperii contractului: ";
                    temp->setSold();
                    ContractCumparare * temp1 =  new ContractCumparare(global_nume,global_datai,global_dataf,temp);
                    contracte.push_back(temp1);
                }catch (runtime_error & e) {
                    cout<<e.what();
                }
            }
            else if (tip == "Casa") {
                FisaCasa * temp = new FisaCasa();
                cin>>*temp;
                try {
                    if (temp->isSold())
                        throw runtime_error("Proprietatea e deja vanduta!");
                    proprietati.push_back(temp);
                    cout<<"Introduceti numele contractului: ";
                    cin>>global_nume;
                    cout<<"Introduceti data finalizarii contractului: ";
                    cin>>global_dataf;
                    cout<<"Introduceti data inceperii contractului: ";
                    temp->setSold();
                    ContractCumparare * temp1 =  new ContractCumparare(global_nume,global_datai,global_dataf,temp);
                    contracte.push_back(temp1);
                }catch (runtime_error & e) {
                    cout<<e.what();
                }
            }
        }
    }
    void vizualizareDupaNume() {
        string nume;
        cout<<"Introduceti nume: ";
        for (auto i:contracte) {
            if (i->getNume()==nume) {
                cout<<*i->getProprietate();
                cout<<endl;
            }
        }
    }
    void afisProp() {

    }
    void afisProfit() {

    }
};

int main() {
 Menu *s = Menu::getInstance();
    int op=-1;
    do {
        cout<<"1. Pentru adaugare proprietate.";
        cout<<"\n2. Pentru a vedea toate proprietatile unei persoane";
        cout<<"\n3. Pentru a afisa proprietatile";
        cout<<"\n4. Pentru a afisa profitul";
        cout<<"\n5. Pentru a adauga un contract\n";
        cin>>op;
        try {
            if (op<0 || op >5)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->adaugareProprietate();
                    break;
                }
                case 2: {
                    s->vizualizareDupaNume();
                    break;
                }
                case 3: {
                    s->afisProp();
                    break;
                }
                case 4: {
                   s->afisProfit();
                    break;
                }
                case 5: {
                    s->adaugareContract();
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);
    
    return 0;
}
