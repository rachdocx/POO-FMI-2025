#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Data {
private:
    int zi,luna,an;
public:

    Data(int zi = 0, int luna = 0, int an = 0) {
        this->zi = zi;
        this->luna = luna;
        this -> an = an;
    }
    ~Data() = default;
    friend ostream & operator << (ostream & os, Data & o) {
        os<<o.zi<<" "<<o.luna<<" "<<o.an<<endl;
        return os;
    }
    friend istream & operator >> (istream & is, Data & o) {
        cout<<"Introduceti ziua: ";
        is>>o.zi;
        cout<<"Introduceti luna: ";
        is>>o.luna;
        cout<<"Introduceti anul: ";
        is>>o.an;
        return is;
    }
    int getLuna() {
        return luna;
    }
};
class Masca{
protected:
      string tip_prot;
        float pret;
public:

    Masca(string tip_prot = "N/A", float pret = 0) {
        this->tip_prot = tip_prot;
        this->pret = pret;
    }
    virtual ~Masca() = default;
    friend ostream & operator << (ostream & os, Masca & obj) {
        os<<"Tipul de protectie este: "<<obj.tip_prot<<endl<<"Pretul este: "<<obj.pret;
        return os;
    }
    friend istream & operator >> (istream & is, Masca & obj) {
        cout<<"Introduceti tipul de protectie: ";
        is>>obj.tip_prot;
        cout<<"Introdcueti pretul: ";
        is>>obj.pret;
        return is;
    }
    float getPret() {
        return pret;
    }
    void setPret(float x) {
        pret=pret+x;
    }
};
class MascaChirurgicala:public Masca {
protected:
    string culoare;
    int nr_pliu;
public:
    MascaChirurgicala(string tip_prot = "N/A",float pret = 0, string culoare ="N/A", int nr_pliu = 0):Masca(tip_prot,pret) {
        this->culoare = culoare;
        this->nr_pliu = nr_pliu;
    }
    virtual ~MascaChirurgicala() =default;
    friend ostream & operator << (ostream & os, MascaChirurgicala & obj) {
        os<<static_cast<Masca&>(obj);
        cout<<endl;
        os<<obj.culoare<<" "<<obj.nr_pliu;
        return os;
    }
    friend istream & operator >> (istream & is, MascaChirurgicala & obj) {
        is>>static_cast<Masca&>(obj);
        cout<<"Introduceti culoarea: ";
        is>>obj.culoare;
        cout<<"Introduceti nr de pliuri: ";
        is>>obj.nr_pliu;
        return is;
    }
};
class MascaChirurgicalaPref:public MascaChirurgicala {
    string preferinta;
    public:
    MascaChirurgicalaPref(string tip_prot = "N/A", float pret = 0, string culoare ="N/A", int nr_pliu = 0, string preferinta ="N/A"):MascaChirurgicala(tip_prot,pret,culoare,nr_pliu) {
        this->preferinta=preferinta;
        this->pret=this->pret+this->pret/2;
    }
    ~MascaChirurgicalaPref() = default;
    friend ostream & operator << (ostream & os, MascaChirurgicalaPref & obj) {
        os<<static_cast<MascaChirurgicala&>(obj);
        os<<"Introduceti preferinta: ";
        os<<obj.preferinta;
        return os;
    }
    friend istream & operator >> (istream & is, MascaChirurgicalaPref & obj) {
        is>>static_cast<MascaChirurgicala&>(obj);
        cout<<endl;
        is>>obj.preferinta;
        return is;
    }
};
class MascaPolicarbonat:public Masca{
    string tip_prindere;
public:
    MascaPolicarbonat(string tip_prot = "N/A",float pret =0, string tip_prindere = "N/A"):Masca(tip_prot,pret) {
        this->tip_prindere=tip_prindere;
    }
    friend ostream & operator << (ostream & os, MascaPolicarbonat & obj) {
        os<<static_cast<Masca&>(obj);
        cout<<endl;
        os<<obj.tip_prindere;
        return os;
    }
    friend istream & operator >> (istream & is, MascaPolicarbonat & obj) {
        is>>static_cast<Masca&>(obj);
        cout<<endl;
        cout<<"Introduceti tipul de prindere: ";
        is>>obj.tip_prindere;
        return is;
    }
};
class Dezinfectant {
    int nr_vir,nr_fun,nr_bac;
    float pret;
    vector<string>ingrediente;
    vector<string>suprafete;
    float eficienta;

    static const float  totalBacterii;
    static const float totalFungi;
    static const float totalVirusi;

public:
    Dezinfectant * getDezinfectant() {
        return this;
    }
    float getEficienta() {
        return eficienta;
    }
    float getPret() {
        return pret;
    }
    Dezinfectant(int nr_vir = 0, int nr_fun =0, int nr_bac = 0,float pret =0, vector<string>ingrediente={}, vector<string>suprafete={}) {
        this->ingrediente=ingrediente;
        this->nr_bac=nr_bac;
        this->pret=pret;
        this->nr_fun=nr_fun;
        this->nr_vir=nr_fun;
        this->suprafete=suprafete;
        this->eficienta =this->nr_bac/totalBacterii + this->nr_fun/nr_fun+this->nr_vir/totalVirusi;
    }
    ~Dezinfectant() = default;
    friend ostream & operator << (ostream & os, Dezinfectant & obj) {
        os<<"Dezinfectantul omoara: "<<obj.nr_fun<<" "<<obj.nr_bac<<" "<<obj.nr_vir<<endl;
        os<<"Pretul dezinfectantului este: "<<obj.pret;
        os<<endl<<"Ingredientele dezinfectantului sunt:\n";
        for (auto i:obj.ingrediente)
            os<<i<<" ";
        os<<"\nSuprafetele pe care poate fi aplicat sunt:\n";
        for (auto i:obj.suprafete)
            os<<i<<" ";
        return os;
    }
    friend istream & operator >> (istream & is, Dezinfectant & obj) {
        cout<<"Introduceti numarul de bacterii, virusuri si fungi: ";
        is>>obj.nr_bac>>obj.nr_vir>>obj.nr_fun;
        cout<<"Introduceti pretul: ";
        is>>obj.pret;
        int n;
        cout<<"Introduceti numarul de ingrediente: ";
        is>>n;
        cout<<"Introduceit ingredientele: ";
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            obj.ingrediente.push_back((temp));
        }
        cout<<"Introduceti numarul de suprafete: ";
        is>>n;
        cout<<"Introduceit suprafete: ";
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            obj.suprafete.push_back((temp));
        }
        return is;
    }

};
class Achizitie {
    Data * data;
    vector<Dezinfectant*> dezinfectanti;
    vector<Masca*> masti;
    string nume;
    float total;
public:
    float getTotal() {
        return total;
    }
    vector<Masca*>getMasti() {
        return masti;
    }
    Achizitie(Data * data = nullptr, vector<Dezinfectant*> dezinfectanti = {}, vector<Masca*> masti= {}, string nume ="N/A", int total=0) {
        this->data=data;
        this->dezinfectanti=dezinfectanti;
        this->masti=masti;
        this->nume = nume;
        this->total = total;
        for (auto i:dezinfectanti)
            total+=i->getPret();
        for (auto i:masti)
            total+=i->getPret();
    }
    ~Achizitie() {
        if (data != nullptr)
            delete data;
        for (auto i:dezinfectanti)
            if (i != nullptr)
                delete i;
        dezinfectanti.clear();
        for (auto i:masti)
            if (i != nullptr)
                delete i;
        masti.clear();
    }
    friend ostream & operator << (ostream & os, Achizitie & obj) {
        os<<*obj.data;
        os<<endl<<"Numele este: "<<obj.nume;
        os<<"\nDezinfectante comandate: \n";
        for (auto i: obj.dezinfectanti)
            os<<i<<endl;
        os<<"\nMasti comandate: \n";
        os<<"\nDezinfectante comandate: \n";
        for (auto i: obj.masti) {
            if (MascaChirurgicala * temp = dynamic_cast<MascaChirurgicala*>(i))
                cout<<temp;
            if (MascaChirurgicalaPref * temp = dynamic_cast<MascaChirurgicalaPref*>(i))
                cout<<temp;
            if (MascaPolicarbonat * temp = dynamic_cast<MascaPolicarbonat*>(i))
                cout<<temp;
        }
        return os;
    }
    friend istream & operator >> (istream & is, Achizitie & obj) {
        cout<<"Introduceti data: \n";
        if (obj.data == nullptr)
            obj.data = new Data();
        is>>*obj.data;
        cout<<"\nIntroduceti cate masti: ";
        int n;
        is>>n;
        cout<<"Introduceti ce masti: ";
        for (int i=1;i<=n;++i) {
            cout<<"Introduceti tipul de masca: ";
            string tip;
            is>>tip;
            if (tip == "MascaChirurgicala") {
                MascaChirurgicala * m = new MascaChirurgicala();
                is>>*m;
                obj.masti.push_back(m);
            }
            if (tip == "MascaChirurgicalaPreferinta") {
                MascaChirurgicalaPref * m = new MascaChirurgicalaPref();
                is>>*m;
                obj.masti.push_back(m);
            }
            if (tip == "MascaPoli") {
                MascaPolicarbonat * m = new MascaPolicarbonat();
                is>>*m;
                obj.masti.push_back(m);
            }
        }
        cout<<"\nIntroduceti cate dezinfectante: ";
        is>>n;
        cout<<"Introduceti ce dezinfectante: ";
        for (int i=1;i<=n;++i) {
            Dezinfectant * d = new Dezinfectant();
            cin>>*d;
            obj.dezinfectanti.push_back(d);
        }
        return is;
    }
    Data * getData() {
        return data;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    inline static Menu * MENU;
    vector<Masca*> masti;
    vector<Dezinfectant*> dezinfectanti;
    vector<Achizitie*> achiztii;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    void adaugareDezinfectantStoc() {
        Dezinfectant * temp = new Dezinfectant();
        cin>>*temp;
        dezinfectanti.push_back(temp);
    }
    void adaugareMascaStoc() {
        Masca * temp = new Masca();
        cin>>*temp;
        masti.push_back(temp);
    }
    void adauagreAchizitie() {
        Achizitie * temp = new Achizitie();
        cin>>*temp;
        achiztii.push_back(temp);
    }
    void dezinfectantEficient() {
        Dezinfectant * ma = dezinfectanti[0];
        float mav = ma->getEficienta();
        for (auto i:dezinfectanti)
            if (i->getEficienta()>mav) {
                mav=i->getEficienta();
                ma=i;
            }
        cout<<*ma;
    }
    void totalLuna(int k) {
        float suma = 0;
        for (auto i:achiztii) {
            Data * d =i->getData();
            if (d->getLuna() == k)
                suma+=i->getTotal();
        }
        cout<<suma;
    }
    void totalMastiPref() {
        float suma =0;
        for (auto i:achiztii) {
            vector<Masca*>  mastia = i->getMasti();
            for (auto j:mastia) {
                if (MascaChirurgicalaPref * temp = dynamic_cast<MascaChirurgicalaPref*>(j))
                    suma+=temp->getPret();
            }
        }
        cout<<suma;
    }
    void modificaReteta() {

    }
    void clientFidel() {

    }
    void ziVenitSlab() {

    }
    void TVA() {

    }
};

int main() {

 Menu *s = Menu::getInstance();
    int op=-1;
    do {
        cin>>op;
        try {
            if (op<0 || op >10)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->adaugareDezinfectantStoc();
                    break;
                }
                case 2: {
                    s->adaugareMascaStoc();
                    break;
                }
                case 3: {
                    s->adauagreAchizitie();
                    break;
                }
                case 4: {
                    s->dezinfectantEficient();
                    break;
                }
                case 5: {
                    cout<<"Citeste luna pe care o vrei: ";
                    int k;
                    cin>>k;
                    s->totalLuna(k);
                    break;
                }
                case 6: {
                    s->totalMastiPref();
                    break;
                }
                case 7: {
                    s->modificaReteta();
                    break;
                }
                case 8: {
                    s->clientFidel();
                    break;
                }
                case 9: {
                    s->ziVenitSlab();
                    break;
                }
                case 10: {
                    s->TVA();
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);
    
    return 0;
}
