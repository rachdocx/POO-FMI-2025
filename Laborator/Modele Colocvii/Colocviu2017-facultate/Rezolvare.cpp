#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Dosar {
protected:
    string nume;
    string seria;
    string nr_ci;
    float medie_bac;

    const int id_dosar;

    static int id_generator;
public:
    Dosar(string nume = "N/A", string seria="N/A", string nr_ci="N/A", float medie_bac= 0):id_dosar(id_generator++) {
        this->medie_bac=medie_bac;
        this->nr_ci=nr_ci;
        this->seria=seria;
        this->nume=nume;

    }

    string nume1() const {
        return nume;
    }

    void set_nume(const string &nume) {
        this->nume = nume;
    }

    string seria1() const {
        return seria;
    }

    void set_seria(const string &seria) {
        this->seria = seria;
    }

    string nr_ci1() const {
        return nr_ci;
    }

    void set_nr_ci(const string &nr_ci) {
        this->nr_ci = nr_ci;
    }

    float medie_bac1() const {
        return medie_bac;
    }

    void set_medie_bac(float medie_bac) {
        this->medie_bac = medie_bac;
    }

    int id_dosar1() const {
        return id_dosar;
    }

    virtual ~Dosar()=default;
    friend ostream & operator << (ostream & os, Dosar & obj) {
        os<<"Numele este: "<<obj.nume;
        os<<"\nSeria de pe buletin este: "<<obj.seria;
        os<<"\nCI este: "<<obj.nr_ci;
        os<<"\nMedia de bac: "<<obj.medie_bac;
        return os;
    }
    friend istream & operator >> (istream & is, Dosar & obj) {
        cout<<"Introduceti numele studentului: ";
        is>>obj.nume;
        cout<<"Introduceti seria de pe buletin: ";
        is>>obj.seria;
        cout<<"Introduceti CI: ";
        is>>obj.nr_ci;
        cout<<"Introfuceti media de bac: ";
        is>>obj.medie_bac;
        return is;
    }
    virtual string getId() const =0;
};
class DosarID:public Dosar {
private:
    float nota_mate;
    float nota_oral;
    float nota_interviu;
public:
    DosarID(string nume = "N/A", string seria="N/A", string nr_ci="N/A", float medie_bac=0, float nota_oral = 0, float nota_interviu = 0):Dosar(nume,seria,nr_ci,medie_bac) {
        this->nota_mate=nota_mate;
        this->nota_oral=nota_oral;
        this->nota_interviu=nota_interviu;
    }
    ~DosarID() = default;
    friend ostream & operator << (ostream & os, DosarID & obj) {
        os<<static_cast<Dosar&>(obj);
        os<<"\nMedia de bac la mate: "<<obj.nota_mate;
        os<<"\nNota interviu este: "<<obj.nota_interviu;
        os<<"\nNota oral este: "<<obj.nota_oral;
    }
    friend istream & operator >> (istream & is, DosarID & obj) {
        is>>static_cast<Dosar&>(obj);
        cout<<"Introduceti nota de bac la mate: ";
        is>>obj.nota_mate;
        cout<<"Introduceti nota oral: ";
        is>>obj.nota_oral;
        cout<<"Introduceti nota interviu: ";
        is>>obj.nota_interviu;
        return is;
    }
    string getId() const {
        string temp = "";
        temp = temp + "ID_" + to_string(this->id_dosar);
        return temp;
    }
};
class DosarID2: public DosarID {
private:
    string domeniu;
    string supliment;
public:
    DosarID2(string nume = "N/A", string seria="N/A", string nr_ci="N/A",float medie_bac=0, float nota_oral = 0, float nota_interviu = 0, string domeniu = "N/A", string supliment="N/A"):DosarID(nume,seria,nr_ci,medie_bac,nota_oral,nota_interviu) {
        this->domeniu=domeniu;
        this->supliment=supliment;
    }
    ~DosarID2() = default;
    friend ostream & operator << (ostream & os, DosarID2 & obj) {
        os<<static_cast<Dosar&>(obj);
        os<<"\nDomeniul studiat anterior: "<<obj.domeniu;
        os<<"\nSupliment medie absolvire: "<<obj.supliment;
    }
    friend istream & operator >> (istream & is, DosarID2 & obj) {
        is>>static_cast<Dosar&>(obj);
        cout<<"Introduceti domeniul studiat anterior: ";
        is>>obj.domeniu;
        cout<<"Introduceti suplimentul mediei de absolvire: ";
        is>>obj.supliment;
        return is;
    }
    string getId() const {
        string temp = "";
        temp = temp + "ID2_" + to_string(this->id_dosar);
        return temp;
    }
};
class DosarIF : public Dosar {
private:
    float p1,p2;
public:
    DosarIF(string nume = "N/A", string seria="N/A", string nr_ci="N/A", float p1 = 1, float p2 = 1):Dosar(nume,seria,nr_ci,medie_bac) {
        this->p1=p1;
        this->p2=p2;
    }
    ~DosarIF();
    friend ostream & operator << (ostream & os, DosarIF & obj) {
        os<<static_cast<Dosar&>(obj);
        os<<"\nNota la proba 1: "<<obj.p1;
        os<<"\nNota la proba 2: "<<obj.p2;
    }
    friend istream & operator >> (istream & is, DosarIF & obj) {
        is>>static_cast<Dosar&>(obj);
        cout<<"Introduceti nota la proba 1: ";
        is>>obj.p1;
        cout<<"Introduceti nota la proba 2: ";
        is>>obj.p2;
        return is;
    }
    string getId() const {
        string temp = "";
        temp = temp + "IF_" + to_string(this->id_dosar);
        return temp;
    }
};
class DosarIF2 : public DosarID2{
private:
public:
    DosarIF2(string nume = "N/A", string seria="N/A", string nr_ci="N/A",float medie_bac=0, float nota_oral = 0, float nota_interviu = 0, string domeniu = "N/A", string supliment="N/A"):DosarID2(nume,seria,nr_ci,medie_bac,nota_oral,nota_interviu,domeniu,supliment) {

    }
    ~DosarIF2() = default;
    friend ostream & operator << (ostream & os, DosarIF2 & obj) {
        os<<static_cast<DosarID2&>(obj);

    }
    friend istream & operator >> (istream & is, DosarIF2 & obj) {
        is>>static_cast<DosarID2&>(obj);
    }
    string getId() const {
        string temp = "";
        temp = temp + "IF2_" + to_string(this->id_dosar);
        return temp;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    static Menu * MENU;
    vector<Dosar*>dosare;
    const int locuriID = 250;
    const int locuriIF = 150;

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

    void adaugareDosar() {
        string tip;
        cout<<"Introduceti tipul de dosar: ";
        cin>>tip;
        if (tip == "ID") {
            DosarID * temp = new DosarID();
            cin>>*temp;
            dosare.push_back(temp);
        }
        else if (tip =="ID2") {
            DosarID2 * temp = new DosarID2();
            cin>>*temp;
            dosare.push_back(temp);
        }
        else if (tip == "IF") {
            DosarIF * temp = new DosarIF();
            cin>>*temp;
            dosare.push_back((temp));
        }
        else if (tip == "IF2") {
            DosarIF2 * temp = new DosarIF2();
            cin>>*temp;
            dosare.push_back(temp);
        }
    }
    void afisareDosare() {
        for (auto i:dosare) {
            if (DosarIF2 * temp = dynamic_cast<DosarIF2*>(i)) {
                cout<<temp->getId()<<endl;
                cout<<*temp;
            }
            if (DosarIF * temp = dynamic_cast<DosarIF*>(i)) {
                cout<<temp->getId()<<endl;
                cout<<*temp;
            }
            if (DosarID2 * temp = dynamic_cast<DosarID2*>(i)) {
                cout<<temp->getId()<<endl;
                cout<<*temp;
            }
            if (DosarID * temp = dynamic_cast<DosarID*>(i)) {
                cout<<temp->getId()<<endl;
                cout<<*temp;
            }
        }
    }
};
int Dosar::id_generator = 0;
Menu* Menu::MENU = nullptr;
int main() {
 Menu *s = Menu::getInstance();
    int op=-1;
    do {

        cin>>op;
        try {
            if (op<0 || op >4)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {

                    break;
                }
                case 2: {

                    break;
                }
                case 3: {

                    break;
                }
                case 4: {

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
