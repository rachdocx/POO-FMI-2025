#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
class Bilet {
protected:
    string statie_plecare;
    string statie_sosire;
    string data;
    string ora;
    string cod_tren;
    int durata;
    int distanta;
    bool accest_rest;
    float pret;
    int clasa;
    const int id_bilet;

    static int id_generator;
public:
    Bilet(string statie_plecare = "N/A", string statie_sosire= "N/A", string data= "N/A", string ora= "N/A", string cod_tren="N/A", int durata = 0 , int distanta = 0, bool acces_rest = false, int clasa =1):id_bilet(id_generator++) {
        this->statie_plecare=statie_plecare;
        this->statie_sosire=statie_sosire;
        this->data=data;
        this->ora=ora;
        this->cod_tren=cod_tren;
        this->durata=durata;
        this->distanta=distanta;
        this->pret=pret;
        this->accest_rest=acces_rest;
        this->clasa=clasa;
    }
    virtual ~Bilet() = default;
    friend ostream & operator << (ostream & os, Bilet & obj) {
        os<<"Statia de plecare este: "<<obj.statie_plecare;
        os<<"\nStatia de sosire este: "<<obj.statie_sosire;
        os<<"\nData plecarii este: "<<obj.data;
        os<<"\nCodul trenului este: "<<obj.cod_tren;
        os<<"\nTren o sa dureze: "<<obj.durata;
        os<<"\nTrenul o sa parcurga o distanta "<<obj.distanta;
        os<<"\nPretul biletului este: "<<obj.pret;
        os<<"\nAre acces trenul la restaurant: "<<obj.accest_rest;
        os<<"\nClasa trenului este: "<<obj.clasa;
        return os;
    }
    friend istream & operator >> (istream & is, Bilet & obj) {
        cout<<"Introdu statia de plecare: ";
        is>>obj.statie_plecare;
        cout<<"Introdu statia de sostire: ";
        is>>obj.statie_sosire;
        cout<<"Introdu data plecarii: ";
        is>>obj.data;
        cout<<"Introdu codul trenului: ";
        is>>obj.cod_tren;
        cout<<"Introdu durata trenului: ";
        is>>obj.durata;
        cout<<"Introdu distanta trenului: ";
        is>>obj.distanta;
        cout<<"Introdu clasa biletului: ";
        is>>obj.clasa;
        return is;
    }
    virtual string getSerieBilet() const =0;
    const string getCodTren() {
        return this->cod_tren;
    }
    const int getDistanta() {
        return this->distanta;

    }
};
class BiletRegio: public Bilet {
private:
public:
    BiletRegio(string statie_plecare = "N/A", string statie_sosire= "N/A", string data= "N/A", string ora= "N/A", string cod_tren="N/A", int durata = 0 , int distanta = 0):Bilet(statie_plecare,statie_sosire,data,ora,cod_tren,durata,distanta) {
        this->pret=0.39*this->distanta;
        this->accest_rest = false;
        try {
            if (this->clasa != 1 && this->clasa!= 2)
                throw runtime_error("Clasa nu exista!");
            if (this->clasa == 1)
                this->pret=(this->pret*20)/100;
        }catch (runtime_error & e) {
            cout<<e.what();
        }
    }
    ~BiletRegio() = default;
    friend ostream & operator << (ostream & os, BiletRegio & obj) {
        os<<static_cast<Bilet&>(obj);
        return os;
    }
    friend istream & operator >> (istream & is, BiletRegio & obj) {
        is>>static_cast<Bilet&>(obj);
        obj.pret=0.39*obj.distanta;
        obj.accest_rest = false;
        try {
            if (obj.clasa != 1 && obj.clasa!= 2)
                throw runtime_error("Clasa nu exista!");
            if (obj.clasa == 1)
                obj.pret=(obj.pret*20)/100;
        }catch (runtime_error & e) {
            cout<<e.what();
        }
        return is;
    }
    string getSerieBilet() const {
        string temp ="R";
        if (this->clasa == 1)
            temp +="I-";
        else
            temp +="II-";
        temp+=to_string(this->id_bilet);
        return temp;
    }
};
class BiletInterRegio:public Bilet {
private:
    int nr_loc;
public:
    BiletInterRegio(string statie_plecare = "N/A", string statie_sosire= "N/A", string data= "N/A", string ora= "N/A", string cod_tren="N/A", int durata = 0 , int distanta = 0, int nr_loc=0):Bilet(statie_plecare,statie_sosire,data,ora,cod_tren,durata,distanta) {
        this->pret=0.7*this->distanta;
        this->accest_rest = true;
        this->nr_loc=nr_loc;
        try {
            if (this->clasa != 1 && this->clasa!= 2)
                throw runtime_error("Clasa nu exista!");
            if (this->clasa == 1)
                this->pret=(this->pret*20)/100;
        }catch (runtime_error & e) {
            cout<<e.what();
        }
    }
    ~BiletInterRegio() = default;
    friend ostream & operator << (ostream & os, BiletInterRegio & obj) {
        os<<static_cast<Bilet&>(obj);
        return os;
    }
    friend istream & operator >> (istream & is, BiletInterRegio & obj) {
        is>>static_cast<Bilet&>(obj);
        obj.pret=0.7*obj.distanta;
        obj.accest_rest = true;
        try {
            if (obj.clasa != 1 && obj.clasa!= 2)
                throw runtime_error("Clasa nu exista!");
            if (obj.clasa == 1)
                obj.pret=(obj.pret*20)/100;
        }catch (runtime_error & e) {
            cout<<e.what();
        }
        return is;
    }
    string getSerieBilet() const {
        string temp ="RI";
        if (this->clasa == 1)
            temp +="I-";
        else
            temp +="II-";
        temp+=to_string(this->id_bilet);
        return temp;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    inline static Menu * MENU;
    vector<Bilet*> bilete;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    ~Menu() {
        for (auto i:bilete)
            delete i;
        bilete.clear();
    }
    void adaugaBilet() {
        Bilet * temp;
        cout<<"Introduceti tipul biletului: ";
        string tip;
        cin>>tip;
        if (tip =="IR") {
            temp = new BiletInterRegio();
            cin>>*temp;
        }else {
            temp = new BiletRegio();
            cin>>*temp;
        }
        bilete.push_back(temp);
    }

    void bileteTren() {
        cout<<"Introduceti codul trenului: ";
        string cod;
        for (auto i:bilete) {
            if (i->getCodTren() == cod) {
                cout<<*i;
            }
        }
    }
    void listareDistante() {
        cout<<"Introduceti o distanta: ";
        int dis;
        cin>>dis;
        for (auto i:bilete) {
            if (i->getDistanta() > dis) {
                cout<<*i;
            }
        }
    }
    void anulareBilet() {
        string serie;
        cout<<"Introduceti seria biletului de sters: ";
        cin>>serie;
        for (int i=0;i<bilete.size();++i)
            if (bilete[i]->getSerieBilet() == serie){
                delete bilete[i];
                bilete.erase(bilete.begin()+i);
                break;
        }
    }
};
int Bilet::id_generator=0;
int main() {
 Menu *s = Menu::getInstance();
    int op=-1;
    do {
        cout<<"1. Eliberare bilet";
        cout<<"\n2. Bilete pe un anumit tren";
        cout<<"\n3. Biletele eliberate mai mari decat o distanta";
        cout<<"\n4. Anulare bilet\n";
        cin>>op;
        try {



            if (op<0 || op >4)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->adaugaBilet();
                    break;
                }
                case 2: {
                    s->bileteTren();
                    break;
                }
                case 3: {
                    s->listareDistante();
                    break;
                }
                case 4: {
                    s->anulareBilet();
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);

    return 0;
}
