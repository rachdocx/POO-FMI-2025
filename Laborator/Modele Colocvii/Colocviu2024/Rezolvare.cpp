#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Item {
protected:
    int pret;
    const int id_item;
    static int id_generator;
public:
    int getId() {
        return  id_item;
    }
    int getPrice() {
        return pret;
    }
    Item(int pret = 0):id_item(id_generator++) {
        this->pret=pret;
    }
    virtual ~Item() = default;
    friend ostream & operator << (ostream & os, Item & obj) {
        os<<"Pret: "<<obj.pret<<endl<<"ID: "<<obj.id_item<<endl;
        return os;
    }
};
class Zid: public Item {
private:
    float hZid,lungimeZid,grosimeZid;
public:

    Zid(int pret = 0, float hZid = 0, float lungimeZid = 0, float grosimeZid = 0) {
        this->pret = pret;
        this->hZid=hZid;
        this->lungimeZid=lungimeZid;
        this->grosimeZid=grosimeZid;
    }
    ~Zid() = default;
    void upgradeZid() {
        this->hZid+=1;
        this->lungimeZid+=1;
        this->grosimeZid+=1;
        this->pret= 100 * this->grosimeZid * this->lungimeZid * this->grosimeZid;
    }

    void setHZid(float x) {
        this->hZid += x;
    }
    void setLungimeZid(float x) {
        this->lungimeZid += x;
    }
    void setGrosimeZid(float x) {
        this->grosimeZid += x;
    }
    friend ostream & operator << (ostream & os, Zid & obj) {
        os<<static_cast<Item&>(obj)<<"Inaltimea este: "<<obj.hZid<<endl<<"Latimea este: "<<obj.lungimeZid<<endl<<"Grosimea este: "<<obj.grosimeZid<<endl;
        return os;
    }
};
class Turn: public Item {
    int putere;
public:
    void upgradeTurn() {
        this->putere+=500;
        this->pret= 500 * this->putere;
    }
    Turn(int pret = 0, int putere = 0) {
        this->pret = pret;
        this->putere = putere;
    }
    ~Turn() = default;
    void setPutere(int x) {
        this->putere+=x;
    }
    friend ostream & operator << (ostream & os, Turn & obj) {
        os<<static_cast<Item&>(obj)<<"Puterea turnului este: "<<obj.putere<<endl;
        return os;
    }
};
class Robot:public Item {
protected:
    int damage, nivel, viata;
public:
    Robot(int pret = 0, int damage = 0, int nivel = 0, int viata = 0) {
        this->pret=pret;
        this->damage=damage;
        this->nivel=nivel;
        this->viata=viata;
    }
    virtual ~Robot()=default;
    void setDamage(int x) {
        this->damage+=x;
    }
    void setNivel(int x) {
        this->nivel+=x;
    }
    void setViata(int x) {
        this->viata+=x;
    }
    friend ostream & operator << (ostream & os, Robot & obj) {
        os<<static_cast<Item&>(obj)<<"Damage-ul este: "<<obj.damage<<endl<<"Nivelul este: "<<obj.nivel<<endl<<"Viata este: "<<obj.viata<<endl;
        return os;
    }
};
class RobotAerian:public Robot {
    int autonomia;
public:
    RobotAerian(int pret = 0, int damage = 0, int nivel = 0, int viata = 0, int autonomia = 0) {
        this->pret=pret;
        this->damage=damage;
        this->nivel=nivel;
        this->viata=viata;
        this->autonomia=autonomia;
    }
    ~RobotAerian() =  default;
    void setAutonomie(int x) {
        autonomia+=x;
    }
    void updateRobotAerian() {
        autonomia+=1;
        nivel++;
        damage+=25;
        pret+= 50*autonomia;
    }
    friend ostream & operator << (ostream & os, RobotAerian & obj) {
        os<<static_cast<Robot&>(obj)<<"Autonomia este: "<<obj.autonomia;
        return os;
    }
};
class RobotTerestru: public Robot {
    int nr_gloate;
    bool scut;
public:
    RobotTerestru(int pret = 0, int damage = 0, int nivel = 0, int viata = 0, int nr_gloate = 0, bool scut = 0) {
        this->pret=pret;
        this->damage=damage;
        this->nivel=nivel;
        this->viata=viata;
        this->nr_gloate=nr_gloate;
        this->scut=scut;
    }
    ~RobotTerestru() = default;
    void setGloante(int x) {
        nr_gloate=x;
    }
    void setScut(bool x) {
        scut=x;
    }
    void updateRobotTerestru() {
        nr_gloate+=100;
        nivel++;
        damage+=50;
        pret+= 10*nr_gloate;
    }
    friend ostream & operator << (ostream & os, RobotTerestru & obj) {
        os<<static_cast<Item&>(obj)<<"Robotul mai are gloante: "<<obj.nr_gloate<<endl<<"Scutul este: "<<obj.scut;
        return os;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    inline static Menu * MENU;
    vector<Item*> inventar;
    int bani;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    void setBalance(int bani) {
        this->bani=bani;
    }
    void changeBalance(int bani) {
        this->bani=this->bani+bani;
    }
    void AdaugaItem() {
        string optiune;
        cout<<"Introdu ce Item vrei sa cumperi: ";
        cin>>optiune;
        if (optiune == "Zid") {
            Item * temp = new Zid(300,2,1,0.5);
            changeBalance(-300);
            inventar.push_back(temp);
        }else if (optiune == "Turn") {
            Item * temp = new Turn(500,1000);
            changeBalance(-500);
            inventar.push_back(temp);
        }
        else if (optiune == "RobotAerian") {
            Item * temp = new RobotAerian(100,100,1,100,10);
            changeBalance(-100);
            inventar.push_back(temp);
        }
        else if (optiune == "RobotTerestru") {
            Item * temp = new RobotTerestru(50,100,1,100,500);
            changeBalance(-50);
            inventar.push_back(temp);
        }
    }
    void upgradeItem() {
        int optiune;
        cout<<"Introduceti ID-ul itemului: ";
        cin>>optiune;
        for (auto i: inventar) {
            if (i->getId() == optiune) {
                if (Zid * temp = dynamic_cast<Zid*>(i)) {
                    temp->upgradeZid();
                }
                if (Turn * temp = dynamic_cast<Turn*>(i)) {
                    temp->upgradeTurn();
                }
                if (RobotAerian * temp = dynamic_cast<RobotAerian*>(i)) {
                    temp->updateRobotAerian();
                }
                if (RobotTerestru * temp = dynamic_cast<RobotTerestru*>(i)) {
                    temp->updateRobotTerestru();
                }
            }
        }
    }
    void afisCrescator() {
        int i,j;
        for (i=0; i<inventar.size();++i)
            for (j=i+1;j<inventar.size();++j)
                if (inventar[i]->getPrice()>inventar[j]->getPrice())
                    swap(inventar[i],inventar[j]);
        for (auto i:inventar) {
            if (Zid * temp = dynamic_cast<Zid*>(i)) {
                cout<<*temp;
            }
            if (Turn * temp = dynamic_cast<Turn*>(i)) {
                cout<<*temp;
            }
            if (RobotAerian * temp = dynamic_cast<RobotAerian*>(i)) {
                cout<<*temp;
            }
            if (RobotTerestru * temp = dynamic_cast<RobotTerestru*>(i)) {
                cout<<*temp;
            }
        }
    }
    void afisRoboti() {
        for (auto i:inventar) {
            if (Robot * temp = dynamic_cast<Robot*>(i))
                cout<<*i;
        }
    }
    void sell() {
        int id,i;
        cout<<"Introduceti id-ul obiectului pe care vreti sa il vindeti: ";
        cin>>id;
        for (i=0;i<inventar.size();++i) {
            if (inventar[i]->getId() == id) {
                delete inventar[i];
                inventar.erase(inventar.begin()+i);
                changeBalance(500);
            }
        }
    }
};
int Item::id_generator=0;
int main() {
 Menu *s = Menu::getInstance();
    s->setBalance(50000);
    int op=-1;
    do {
        cout<<"1. Pentru a adauga un item\n";
        cout<<"2. Pentru a upgrada un item\n";
        cout<<"3. Pentru a afisa in ordine crescatoare itemele\n";
        cout<<"4. Pentru a afisa toti robotii din inventar\n";
        cout<<"5. Pentru a vinde un item\n";

        cin>>op;
        try {
            if (op<0 || op >5)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->AdaugaItem();
                    break;
                }
                case 2: {
                    s->upgradeItem();
                    break;
                }
                case 3: {
                    s->afisCrescator();
                    break;
                }
                case 4: {
                   s->afisRoboti();
                    break;
                }
                case 5: {
                    s->sell();
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);
    
    return 0;
}
