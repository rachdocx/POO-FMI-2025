#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;
class Masina {
protected:
    int an;
    string nume;
    int max_speed;
    int greutate;
    int autonomie;
public:
    void setSpeed(int k) {
        this->max_speed=this->max_speed+(this->max_speed*k)/100;
    }
    string getNume(){
        return nume;
    }
    int getAutonomie() {
        return autonomie;
    }
    Masina(int an = 0, string nume = "N/A", int max_speed =0, int greutate = 0) {
        this->an=an;
        this->nume=nume;
        this->max_speed=max_speed;
        this->greutate=greutate;
    }
    virtual ~Masina() = default;
    friend ostream & operator << (ostream & os, Masina & obj) {
        os<<"Numele masinii este: "<<obj.nume<<endl;
        os<<"Anul de fabricatie este: "<<obj.an<<endl;
        os<<"Viteza maxima este: "<<obj.max_speed<<endl;
        os<<"Greutatea este: "<<obj.greutate<<endl;
        return os;
    }
    friend istream & operator >> (istream & is, Masina & obj) {
        cout<<"Introdocueti anul de fabricatie: ";
        is>>obj.an;
        cout<<"\nIntrodcueti numele: ";
        is>>obj.nume;
        cout<<"\nIntroduceti viteza maxima: ";
        is>>obj.max_speed;
        cout<<"\nIntroduceti greutatea: ";
        is>>obj.greutate;
        cout<<endl;
        return is;
    }
};
class MasinaFosil:virtual public Masina {
protected:
    string tip_combustibil;
    int capacitate;
public:
    MasinaFosil(int an = 0, string nume = "N/A", int max_speed =0, int greutate = 0, string tip_combustibil = "N/A", int capacitate = 0):Masina(an,nume,max_speed,greutate) {
        this->capacitate=capacitate;
        this->tip_combustibil=tip_combustibil;
        this->autonomie=this->capacitate/sqrt(this->greutate);
    }
    ~MasinaFosil()=default;
    friend ostream & operator << (ostream & os, MasinaFosil & obj) {
        os<<static_cast<Masina&>(obj);
        os<<"Tipul de combustibil este: "<<obj.tip_combustibil;
        os<<"\nCapacitatea rezervorului este: "<<obj.capacitate;
        os<<"\nAutonomia masinii este"<<obj.autonomie;
        return os;
    }
    friend istream & operator >> (istream & is, MasinaFosil & obj) {
        is>>static_cast<Masina&>(obj);
        cout<<"Introduceti tipul de combustibil: ";
        is>>obj.tip_combustibil;
        cout<<"Introduceti capacitatea rezervorului: ";
        is>>obj.capacitate;
        obj.autonomie=obj.capacitate/sqrt(obj.greutate);
        return is;
    }

};
class MasinaElectric:virtual public Masina {
protected:
    int capacitate_baterie;
public:
    MasinaElectric(int an = 0, string nume = "N/A", int max_speed =0, int greutate = 0, int capacitate_baterie=0):Masina(an,nume,max_speed,greutate) {
        this->capacitate_baterie=capacitate_baterie;
        this->autonomie=this->capacitate_baterie/(this->greutate*this->greutate);
    }
    ~MasinaElectric()=default;
    friend ostream & operator << (ostream & os, MasinaElectric & obj) {
        os<<static_cast<Masina&>(obj);
        os<<"\nCapacitatea Bateriei este: "<<obj.capacitate_baterie;
        os<<"\nAutonomia masinii este: "<<obj.autonomie;
        return os;
    }
    friend istream & operator >> (istream & is, MasinaElectric & obj) {
        is>>static_cast<Masina&>(obj);
        cout<<"Introduceti capacitatea bateriei: ";
        is>>obj.capacitate_baterie;

        obj.autonomie=obj.capacitate_baterie/(obj.greutate*obj.greutate);
        return is;
    }
};
class MasinaHibrid: public MasinaElectric, public MasinaFosil {
public:
    MasinaHibrid(int an = 0, string nume = "N/A", int max_speed =0, int greutate = 0, string tip_combustibil = "N/A", int capacitate = 0, int capacitate_baterie = 0):MasinaFosil(an,nume,max_speed,greutate,tip_combustibil,capacitate),MasinaElectric(an,nume,max_speed,greutate,capacitate_baterie) {

    }
    ~MasinaHibrid() = default;
    friend ostream & operator << (ostream & os, MasinaHibrid & obj) {
        os<<static_cast<MasinaFosil&>(obj);
        os<<"\nCapacitatea Bateriei este: "<<obj.capacitate_baterie;

        return os;
    }
    friend istream & operator >> (istream & is, MasinaHibrid & obj) {
        is>>static_cast<MasinaFosil&>(obj);
        cout<<"Introduceti capacitatea bateriei: ";
        is>>obj.capacitate_baterie;
        obj.autonomie=obj.capacitate_baterie/(obj.greutate*obj.greutate) + obj.capacitate/sqrt(obj.capacitate);
        return is;
    }
};
class Tranzactie {
private:
    string nume;
    int zi,luna,an;
    vector <Masina*> masini;
public:
    vector<Masina*> * getMasini() {
        return &masini;
    }
    Tranzactie(string nume = "N/A", int zi=0, int luna =0, int an=0, vector<Masina*>masini={}) {
        this->nume=nume;
        this->luna=luna;
        this->zi=zi;
        this->an=an;
        this->masini=masini;
    }
    ~Tranzactie() {
        for (auto i:masini)
            if (i != nullptr)
                delete i;
        masini.clear();
    }
    friend istream & operator >> (istream & is, Tranzactie & obj) {
        cout<<"Introduceti numele: ";
        is>>obj.nume;
        cout<<"Introduceti data: ";
        is>>obj.zi>>obj.luna>>obj.an;
        int n;
        cout<<"Introduceti cate masini: ";
        is>>n;
        for (int i=1;i<=n;++i) {
            string tip;
            cout<<"Introduceti tipul de masina: ";
            cin>>tip;
            if (tip == "Electrica") {
                MasinaElectric * temp = new MasinaElectric();
                is>>*temp;
                obj.masini.push_back(temp);
            }
            if (tip == "Fosil") {
                MasinaFosil * temp = new MasinaFosil();
                is>>*temp;
                obj.masini.push_back(temp);
            }
            if (tip == "Hibrid") {
                MasinaHibrid * temp = new MasinaHibrid();
                is>>*temp;
                obj.masini.push_back(temp);
            }
        }

        return is;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    static Menu * MENU;
    vector<Tranzactie*> tranzactie;
    vector<Masina*> stoc;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    void adaugareMasina() {
        string tip;
        cout<<"Introduceti tipul de masina: ";
        cin>>tip;
        if (tip == "Electrica") {
            MasinaElectric * temp = new MasinaElectric();
            cin>>*temp;
            stoc.push_back(temp);
        }
        if (tip == "Fosil") {
            MasinaFosil * temp = new MasinaFosil();
            cin>>*temp;
            stoc.push_back(temp);
        }
        if (tip == "Hibrid") {
            MasinaHibrid * temp = new MasinaHibrid();
            cin>>*temp;
            stoc.push_back(temp);
        }
    }
    void adaugareTranzactie() {
        Tranzactie * temp = new Tranzactie();
        cin>>*temp;
        tranzactie.push_back(temp);
    }
    void afiseazaModel() {
        map <string,int>frecventa;
        for (auto i:tranzactie) {
            vector<Masina*> * masini = i->getMasini();
            for (auto j:*masini) {
                frecventa[j->getNume()]++;
            }
        }
        int ma=0;
        string nume;
        for (auto i:frecventa) {
            if (i.second > ma) {
                ma=i.second;
                nume=i.first;
            }
        }
        cout<<nume;
    }
    void autonomieMax() {
        int ma=0;
        Masina * temp = nullptr;
        for (auto i:stoc) {
            if (i->getAutonomie() > ma) {
                ma=i->getAutonomie();
                temp = i;
            }
        }
        if (MasinaHibrid * temp1 = dynamic_cast<MasinaHibrid*>(temp)) {
            cout<<*temp1;
        }else
        if (MasinaFosil * temp1 = dynamic_cast<MasinaFosil*>(temp)) {
            cout<<*temp1;
        }else
        if (MasinaElectric * temp1 = dynamic_cast<MasinaElectric*>(temp)) {
            cout<<*temp1;
        }
    }
    void upgradeMasina(int k, string model) {
        for (auto i:stoc) {
            if (i->getNume() == model) {
                i->setSpeed(k);
                break;
            }
        }

    }

};
Menu* Menu::MENU=nullptr;
int main() {
 Menu *s = Menu::getInstance();
    int op=-1;
    do {
        cout<<"1. Adaugati masina pe stoc.\n";
        cout<<"2. Adaugati tranzactie.\n";
        cout<<"3. Afisati cel mai vandut model\n";
        cout<<"4. Afisati modelul cu cea mai mare autonomie\n";
        cout<<"5. Upgrade la o masina\n";
        cin>>op;
        try {
            if (op<0 || op >5)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->adaugareMasina();
                    break;
                }
                case 2: {
                    s->adaugareTranzactie();
                    break;
                }
                case 3: {
                    s->afiseazaModel();
                    break;
                }
                case 4: {
                    s->autonomieMax();
                    break;
                }
                case 5: {
                    cout<<"Introduceti modelul si procentul: ";
                    int k;
                    string model;
                    cin>>k>>model;
                    s->upgradeMasina(k,model);
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);
    return 0;
}
