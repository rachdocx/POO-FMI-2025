#include <iostream>
#include <string>
using namespace std;

class Produs {
    string denProd;
    float preProd;
public:
    Produs(string denProd = "N/A", float predProd = 0 ) {
        this->denProd = denProd;
        this->preProd = predProd;
    }
    ~Produs() = default;
    friend ostream & operator << (ostream & os, Produs & obj) {

        os<<obj.denProd<<" "<<obj.preProd;
        return os;
    }
    friend istream & operator >> (istream & is, Produs & obj) {
        is>>obj.denProd>>obj.preProd;
        return is;
    }
};
class Data {
private:
    int zi, luna, an;
public:
    Data(int zi = 1, int luna =1, int an = 1980) {
        this->zi=zi;
        this->luna=luna;
        this->an=an;
    }
    ~Data() = default;
    friend istream & operator>>(istream &is, Data & obj) {
        cout<<"Introduceti ziua luna anu! ";
        is>>obj.zi>>obj.luna>>obj.an;
        return is;
    }
    int getZi() {
        return zi;
    }
    int getLuna() {
        return luna;
    }
    int getAn() {
        return an;
    }
    friend ostream & operator << (ostream & os, Data & obj) {
        os<<obj.zi<<" "<<obj.luna<<" "<<obj.an;
        return os;
    }
};
class Comanda {
    int Num, nrPortii;
    Produs * Prod;
    Data * d;
public:
    Comanda(int Num = 0, int nrPortii = 0, Produs * Prod = nullptr, Data * d = nullptr) {
        this->Num = Num;
        this->nrPortii = nrPortii;
        this->Prod = Prod;
        this->d = d;
    }
    virtual ~Comanda() {
        if ( Prod != nullptr)
            delete Prod;
        if ( d != nullptr)
            delete d;
    }
    Data * getData() {
        return d;
    }
    friend ostream & operator << (ostream & os, Comanda & obj) {
        os<<*obj.d<<endl<<*obj.Prod<<endl<<obj.Num<<" "<<obj.nrPortii;
        return os;
    }
    friend istream & operator >> (istream & is, Comanda & obj) {
        is>>*obj.d >> *obj.Prod>>obj.Num>>obj.nrPortii;
        return is;
    }
};
class Ospatar {
protected:
    string Nume;
    vector<Comanda*> comenzi;
    int nrComenzi;
    char gen;
    int varsta;
public:
    Ospatar(string Nume = "N/A", vector<Comanda*> comenzi ={},int nrComenzi =0, char gen='0', int varsta = 0) {
        this->Nume = Nume;
        this->comenzi = comenzi;
        this->nrComenzi = nrComenzi;
        this->gen = gen;
        this->varsta = varsta;
    }
    ~Ospatar() {
        for (auto i:comenzi)
            delete i;
        comenzi.clear();
    }
    friend ostream & operator << (ostream & os, Ospatar & obj) {
        os<<
        return os;
    }
    friend istream & operator >> (istream & is, Ospatar & obj) {
        is>>*obj.d >> *obj.Prod>>obj.Num>>obj.nrPortii;
        return is;
    }
};
class CompSpec:public Comanda{
    float pretSupl;
    string ObsC;
public:
    CompSpec(int Num = 0, int nrPortii = 0, Produs * Prod = nullptr, Data * d = nullptr, float pretSupl = 0, string ObsC ="N/A"):Comanda(Num,nrPortii,Prod,d) {
        this->pretSupl = pretSupl;
        this->ObsC = ObsC;
    }
    ~CompSpec() = default;
    friend ostream & operator << (ostream & os, CompSpec & obj) {
        os<<static_cast<Comanda&>(obj)<<endl<<obj.pretSupl<<" "<<obj.ObsC;
        return os;
    }
    friend istream & operator >> (istream & is, CompSpec & obj) {
        is>>static_cast<Comanda&>(obj)>>obj.pretSupl>>obj.ObsC;
        return is;
    }
};
template <class T>
class CompOnline {
    string adrLivr;
    T * comanda;
    int comLivr;
public:
    CompOnline(string adrLivr = "N/A", int comLivr=0, T* comanda =nullptr) {
        this->adrLivr = adrLivr;
        this->comanda = comanda;
        this->comLivr = comLivr;
    }
    ~CompOnline() {
        if (comanda != nullptr)
            delete comanda;
    }
    T * getComanda() {
        return comanda;
    }
    friend ostream & operator << (ostream & os, CompOnline & obj) {
        os<<static_cast<T&>(obj)<<endl<<" "<<obj.adrLivr<<" "<<obj.comLivr;
        return os;
    }
    friend istream & operator >> (istream & is, CompOnline & obj) {
        is>>static_cast<T&>(obj) >> obj.adrLivr >> obj.comLivr;
        return is;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    inline static Menu * MENU;
    vector<Comanda*> comenzi;
    vector<Ospatar*> ospatari;
    vector<CompOnline<Comanda>*> comenziOnline;
    vector<CompOnline<CompSpec>*> comenziOnlineSpec;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    void AdaugareComanda();
    void AdaugareOspatari();
    void AfisareComenzi();
    void AfisarePortii();
    void AfisarreProcent();
    void AfisareOspatar();
};
void Menu::AdaugareComanda() {
    string tip,den;
    int pret,nrportii;
    cout<<"Introduceti tip comanda: ";
    cin>>tip;
    cout<<"\nIntroduceti denumirea si pretu: ";
    cin>>den>>pret;
    cout<<"\nIntroduceti data si nr de portii";
    cin>>nrportii;
    Produs * temp = new Produs(den,pret);
    Data *d = new Data();
    cin>>*d>>nrportii;
    Comanda *cs = new Comanda(3,nrportii,temp,d);
    if (tip == "ComandaSimpla") {
        comenzi.push_back(cs);
    }
    else if (tip == "CompSpeciala") {
        float pret;
        string cerinta;
        cout<<"\nIntroduceti pret si cerinta: ";
        cin>>pret>>cerinta;
        CompSpec *cs = new CompSpec(3,nrportii,temp,d,pret,cerinta);
        comenzi.push_back(cs);
    }
    else if (tip =="ComSimplaOnline") {
        string adresa;
        int comision;
        cout<<"\nIntroduceti adresa si comisionu: ";
        cin>>adresa>>comision;
        CompOnline<Comanda>* temp= new CompOnline<Comanda>(adresa,comision,cs);
        comenziOnline.push_back(temp);
    }
    else if (tip == "ComSpecOnline") {
        string adresa,cerinta;
        int comision,pret;
        cout<<"\nIntroduceti adresa, comision, cerinta si pret: ";
        cin>>adresa>>comision>>cerinta>>pret;
        CompSpec * temp1 = new CompSpec(2,nrportii,temp,d,pret,cerinta);
        CompOnline<CompSpec>* temp2= new CompOnline<CompSpec>(adresa,comision,temp1);
        comenziOnlineSpec.push_back(temp2);
    }
}


int main() {
    Menu *s = Menu::getInstance();
    s->AdaugareComanda();

    return 0;
}
