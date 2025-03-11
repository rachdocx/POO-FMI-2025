#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
//gestiunea sistemului de transport public proiect poo
using namespace std;
int op, id;
char nume[50];
class Statie {
private:
    int id;
    char nume[50];
public:
    Statie() {
        id=0;
        strcpy(nume,"");
    }
    Statie(int id, char nume[]) {
        this->id=id;
        strcpy(this->nume,nume);
    }
    Statie(const Statie &s) {
        this->id=s.id;
        strcpy(this->nume,s.nume);
    }
    int getId() {
        return id;
    }
    void print() {
        cout<<this->id<<" "<<this->nume<<endl;
    }
    void printFile(ofstream &f) {
        f<<this->id<<" "<<this->nume<<endl;
    }

};
void loadStatii(vector<Statie> &statii) {
    ifstream f("statii_metrou.txt");
    while (f>>id) {
        f>>nume;
        statii.push_back(Statie(id,nume));
    }
    f.clear();
    f.close();
}
void saveStatii(vector<Statie> &statii) {
    ofstream f("statii_metrou.txt");
    for (int i=0; i<statii.size(); i++) {
        statii[i].printFile(f);
    }
    f.close();

}
int main() {
    cout<<"1. Adaugare statie"<<endl;
    cout<<"2. Afisare Statii"<<endl;
    cout<<"0 Exit"<<endl;
    cin>>op;
    vector<Statie> statii;
    loadStatii(statii);
    while (op) {
        switch (op) {
            case 1:
                cout<<"Introduceti ID-ul statiei: ";
                cin>>id;
                cout<<"Introduceti numele statiei: ";
                cin>>nume;
                statii.push_back(Statie(id,nume));
                break;
            case 2:
                for (int i=0; i<statii.size(); i++) {
                    statii[i].print();
                }
                break;
            case 3:
                cout<<"Introduceti ID-ul statiei: ";
                cin>>id;
                for (int i=0; i<statii.size(); i++) {
                    if (id==statii[i].getId()) {
                        statii.erase(statii.begin()+i);
                    }
                }
            break;
        }
        cout<<"1. Adaugare statie"<<endl;
        cout<<"2. Afisare Statii"<<endl;
        cout<<"0 Exit"<<endl;
        cin>>op;
    }
    saveStatii(statii);
    return 0;
}