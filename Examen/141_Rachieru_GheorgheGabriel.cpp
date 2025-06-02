#include <iostream>
#include <fstream>
#include <vector>
#include <string>
/*Rachieru Gheorghe Gabriel 141
Apple clang version 17.0.0 (clang-1700.0.13.3)
Radu Vrianceanu */
using namespace std;
class Produs {
protected:
    string nume;
    float gramaj;
public:
    Produs(string nume ="N/A", float gramaj = 0){
        this->nume = nume;
        this->gramaj=gramaj;
    }
    virtual ~Produs() = default;
    friend ostream & operator << (ostream & os, Produs & obj) {
        os<<"Numele produsului este: "<<obj.nume;
        os<<"\nGramajul produsului este: "<<obj.gramaj;
        return os;
    }
    friend istream & operator >> (istream & is, Produs & obj) {
        cout<<"Introduceti numele produsului: ";
        is>>obj.nume;
        cout<<"Introduceti gramajul produslui: ";
        is>>obj.gramaj;
        return is;
    }
    virtual float getEnergie() const =0;
};
class Bautura: public Produs {
    private:
    bool is_sticla;
public:
    Bautura(string nume ="N/A", float gramaj = 0, bool is_sticla = 0):Produs(nume,gramaj) {
        this->is_sticla=is_sticla;
    }
    ~Bautura() = default;
    friend ostream & operator << (ostream & os, Bautura & obj) {
        os<<static_cast<Produs&>(obj);
        os<<"\nBautura este la PET?: "<<obj.is_sticla;
        return os;
    }
    friend istream & operator >> (istream & is,Bautura & obj) {
        is>>static_cast<Produs&>(obj);
        cout<<"Este bautura la PET?: ";
        is>>obj.is_sticla;
        return is;
    }
    float getEnergie() const {
        if (this->is_sticla)
            return 25;
        else
            return 0.25*this->gramaj;
    }
};
class Desert : public Produs {
private:
    string format;
public:
    Desert(string nume ="N/A", float gramaj = 0, string format = "N/A"):Produs(nume,gramaj) {
        this->format=format;
    }
    ~Desert() = default;
    friend ostream & operator << (ostream & os, Desert & obj) {
        os<<static_cast<Produs&>(obj);
        os<<"\nFormatul desertului este: "<<obj.format;
        return os;
    }
    friend istream & operator >> (istream & is, Desert & obj) {
        is>>static_cast<Produs&>(obj);
        cout<<"Introduceti formatul: ";
        is>>obj.format;
        return is;
    }
    float getEnergie() const {
        if (this->format == "Felie")
            return 25;
        else if (this->format == "Portie")
            return 0.5*this->gramaj;
        else if (this->format == "Cupa")
            return 2*this->gramaj;
        return 0;
    }

};
class Burger: public Produs {
private:
    vector<string> ingrediente;
public:
   Burger(string nume ="N/A", float gramaj = 0, vector<string> ingrediente = {}):Produs(nume,gramaj) {
        this->ingrediente=ingrediente;
    }
    ~Burger() = default;
    friend ostream & operator << (ostream & os, Burger & obj) {
        os<<static_cast<Produs&>(obj);
        cout<<"\nBurger-ul are urmatoarele ingrediente: ";
        for (auto i:obj.ingrediente) {
            os<<i<<" ";
        }
        return os;
    }
    friend istream & operator >> (istream & is, Burger & obj) {
        is>>static_cast<Produs&>(obj);
        int n;
        cout<<"Introduceti cate ingrediente are burger-ul: ";
        is>>n;
        for (int i=1;i<=n;++i) {
            cout<<"Introduceti ingredient: ";
            string temp;
            is>>temp;
            obj.ingrediente.push_back(temp);
        }
        return is;
    }
    float getEnergie() const {
        return this->gramaj*ingrediente.size();
    }
};
class Comanda {
private:
    vector<Produs*>produse;
    const int id_comanda;
    bool proritara;
    float energie;
    static int id_generator;
public:
    Comanda(vector<Produs*>produse = {}, bool prioritara = 0, float energie = 0):id_comanda(id_generator++) {
        this->produse=produse;
        this->proritara=prioritara;
        this->energie=energie;
    }
    ~Comanda() {
        for (auto i:produse) {
            delete i;
        }
        produse.clear();
    }
    float getEnergie() {
        float suma;
        for (auto i:produse) {
            if (Bautura * temp = dynamic_cast<Bautura*>(i))
                suma+=temp->getEnergie();
            if (Burger * temp = dynamic_cast<Burger*>(i))
                suma+=temp->getEnergie();
            if (Desert * temp = dynamic_cast<Desert*>(i))
                suma+=temp->getEnergie();

        }
        return suma;
    }
    float getEnergie1() {
        return energie;
    }
    float setEnergie(float x) {
        this->energie+=x;
    }
};
class Angajat {
protected:
    float energie;
public:
    Angajat(float energie = 100) {
        this->energie = energie;
    }
    virtual ~Angajat() = default;
    virtual string getType() const =0 ;
    float getEnergie() {
        return energie;
    }
    float setEnergie(float x) {
        try {
            if (this->energie+x<0)
                throw runtime_error("nu se poate");
            this->energie+=x;
        }
        catch (runtime_error & e) {
            cout<<e.what();
        }
    }
};
class Livrator :public Angajat {
public:
    Livrator(float energie = 100):Angajat(100) {
        this->energie=energie;
    }
    ~Livrator() = default;
    string getType() const {
        return "Livrator";
    }

};
class Casier :public Angajat {
public:
    Casier(float energie = 100):Angajat(100) {
            this->energie=energie;
    }
    ~Casier() = default;
    string getType() const {
        return "Casier";
    }
};
class Bucatar :public Angajat {
public:
    Bucatar(float energie = 100):Angajat(100) {
        this->energie=energie;
    }
    ~Bucatar() = default;
    string getType() const {
        return "Bucatar";
    }
};
int Comanda::id_generator = 0;
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    static Menu * MENU;
    vector<Angajat*> angajati = {new Livrator(100), new Bucatar(100), new Bucatar(100), new Casier(100)};
    vector<Comanda*> comenzi;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    static void removeInstance() {
        if (MENU != nullptr) {
            delete MENU;
            MENU = nullptr;
        }
    }

    void afisPersonal() {
        int nrb=0,nrl=0,nrc=0;
        for (auto i:angajati)
            if (i->getType() == "Bucatar")
                nrb++;
            else if (i->getType() == "Livrator")
                nrl++;
            else if (i->getType() == "Casier")
                nrc++;
        cout<<"Numarul de bucatari este: "<<nrb<<endl;
        cout<<"Numarul de livratori este: "<<nrl<<endl;
        cout<<"Numarul de casieri este: "<<nrc<<endl;
    }
    void ciclu() {
        cout<<"===START====";
        int ok=2;
        int comanda;
        while (ok) {
            cout<<"Adaugati comanda?(1/0)";
            cin>>comanda;
            if (comanda == 1) {
                vector<Produs*> produses;
                cout<<"Introduceti cate produse: ";
                int n;
                cin>>n;
                for (int i=1;i<=n;++i) {
                    string tip;
                    cout<<"Introduceti tipul produsului: ";
                    cin>>tip;
                    if (tip == "Desert") {
                        Desert * temp1 = new Desert();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                    if (tip == "Bautura") {
                        Bautura * temp1 = new Bautura();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                    if (tip == "Burger") {
                        Burger * temp1 = new Burger();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                }
                bool pro;
                cout<<"Este comanda prioritara: ";
                cin>>pro;
                Comanda * temp = new Comanda(produses, pro);
                comenzi.push_back(temp);
            }
            else
                if (comanda == 0)
                    ok=0;
        }
        vector<float> energii;
        for (auto i:comenzi) {
            cout<<"Energiile necesare sunt: ";
            cout<<i->getEnergie()<<endl;
            i->setEnergie(i->getEnergie());
        }
        bool ok1=1;
        for (auto i:comenzi) {
            cout<<i->getEnergie()<<endl;
            for (auto j:angajati) {
                if (ok1==1) {
                    if (j->getType() == "Bucatar") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Livrator") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Casier") {
                        j->setEnergie(-25);
                        i->setEnergie(-100);
                    }
                }
                if (i->getEnergie() < 100) {
                    if (j->getType() == "Bucatar") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Livrator") {
                        j->setEnergie(-25);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Casier") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                }
                if (j->getType() == "Bucatar") {
                    j->setEnergie(-200);
                    i->setEnergie(-200);

                }
                if (j->getType() == "Livrator") {
                    j->setEnergie(-100);
                    i->setEnergie(-100);
                }
                if (j->getType() == "Casier") {
                    j->setEnergie(-100);
                    i->setEnergie(-100);
                }
            }
        }
    }
    void comenziP() {
        cout<<"===START====";
        int ok=2;
        int comanda;
        while (ok) {
            cout<<"Adaugati comanda?(1/0)";
            cin>>comanda;
            if (comanda == 1) {

                vector<Produs*> produses;
                cout<<"Introduceti cate produse: ";
                int n;
                cin>>n;
                for (int i=1;i<=n;++i) {
                    string tip;
                    cout<<"Introduceti tipul produsului: ";
                    cin>>tip;
                    if (tip == "Desert") {
                        Desert * temp1 = new Desert();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                    if (tip == "Bautura") {
                        Bautura * temp1 = new Bautura();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                    if (tip == "Burger") {
                        Burger * temp1 = new Burger();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                }
                bool pro;
                cout<<"Este comanda prioritara: ";
                cin>>pro;

                    Comanda * temp = new Comanda(produses, pro);
                if (!pro)
                 comenzi.push_back(temp);
                else
                    for (int i=comenzi.size();i>0;++i)
                        comenzi[i+1]=comenzi[i];        //se adaugam la inceput daca sunt prioritare;
                comenzi[0]=temp;
            }
            else
                if (comanda == 0)
                    ok=0;
        }
        vector<float> energii;
        for (auto i:comenzi) {
            cout<<"Energiile necesare sunt: ";
            cout<<i->getEnergie()<<endl;
            i->setEnergie(i->getEnergie());
        }
        bool ok1=1;
        for (auto i:comenzi) {
            cout<<i->getEnergie()<<endl;
            for (auto j:angajati) {
                if (ok1==1) {
                    if (j->getType() == "Bucatar") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Livrator") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Casier") {
                        j->setEnergie(-25);
                        i->setEnergie(-100);
                    }
                }
                if (i->getEnergie() < 100) {
                    if (j->getType() == "Bucatar") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Livrator") {
                        j->setEnergie(-25);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Casier") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                }
                if (j->getType() == "Bucatar") {
                    j->setEnergie(-200);
                    i->setEnergie(-200);

                }
                if (j->getType() == "Livrator") {
                    j->setEnergie(-100);
                    i->setEnergie(-100);
                }
                if (j->getType() == "Casier") {
                    j->setEnergie(-100);
                    i->setEnergie(-100);
                }
            }
        }
    }
    void optCiclu(){
    }
    void sortare() {
                cout<<"===START====";
        int ok=2;
        int comanda;
        while (ok) {
            cout<<"Adaugati comanda?(1/0)";
            cin>>comanda;
            if (comanda == 1) {

                vector<Produs*> produses;
                cout<<"Introduceti cate produse: ";
                int n;
                cin>>n;
                for (int i=1;i<=n;++i) {
                    string tip;
                    cout<<"Introduceti tipul produsului: ";
                    cin>>tip;
                    if (tip == "Desert") {
                        Desert * temp1 = new Desert();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                    if (tip == "Bautura") {
                        Bautura * temp1 = new Bautura();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                    if (tip == "Burger") {
                        Burger * temp1 = new Burger();
                        cin>>*temp1;
                        produses.push_back(temp1);
                    }
                }
                bool pro;
                cout<<"Este comanda prioritara: ";
                cin>>pro;
                Comanda * temp = new Comanda(produses, pro);
                comenzi.push_back(temp);
            }
            else
                if (comanda == 0)
                    ok=0;
        }
        vector<float> energii;
        for (int i=0;i<comenzi.size();i++)
            for (int j=i+1;j<comenzi.size();j++)
                if (comenzi[i]->getEnergie1() > comenzi[j]->getEnergie1()) {  //le sortam dupa dificultate
                    swap(comenzi[i],comenzi[j]);
                }
        for (auto i:comenzi) {
            cout<<"Energiile necesare sunt: ";
            cout<<i->getEnergie()<<endl;
            i->setEnergie(i->getEnergie());
        }
        bool ok1=1;
        for (auto i:comenzi) {
            cout<<i->getEnergie()<<endl;
            for (auto j:angajati) {
                if (ok1==1) {
                    if (j->getType() == "Bucatar") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Livrator") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Casier") {
                        j->setEnergie(-25);
                        i->setEnergie(-100);
                    }
                }
                if (i->getEnergie() < 100) {
                    if (j->getType() == "Bucatar") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Livrator") {
                        j->setEnergie(-25);
                        i->setEnergie(-100);
                    }
                    if (j->getType() == "Casier") {
                        j->setEnergie(-100);
                        i->setEnergie(-100);
                    }
                }
                if (j->getType() == "Bucatar") {
                    j->setEnergie(-200);
                    i->setEnergie(-200);

                }
                if (j->getType() == "Livrator") {
                    j->setEnergie(-100);
                    i->setEnergie(-100);
                }
                if (j->getType() == "Casier") {
                    j->setEnergie(-100);
                    i->setEnergie(-100);
                }
            }
        }
    }
};
Menu* Menu::MENU = nullptr;
int main() {
 Menu *s = Menu::getInstance();
    int op=-1;
    do {
        cout<<"1. Afisare angajati";
        cout<<"2. Afisare ciclu\n";
        cout<<"3. Afisare ciclu prioritar\n";
        cout<<"4. Afisare ciclu optimizat\n";
        cout<<"2. Afisare ciclu sortat\n";
        cin>>op;
        try {
            if (op<0 || op >5)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->afisPersonal();
                    break;
                }
                case 2: {
                    s->ciclu();
                    break;
                }
                case 3: {
                    s->comenziP();
                    break;
                }
                case 4: {
                    s->optCiclu();
                    break;
                }
                case 5: {
                    s->sortare();
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);
    Menu::removeInstance();
    return 0;
}
