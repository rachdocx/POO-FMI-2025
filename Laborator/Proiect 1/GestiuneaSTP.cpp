#include <iostream>
#include <fstream>
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
    Statie(int id, const char nume[]) {
        this->id=id;
        strcpy(this->nume,nume);
    }
    Statie(const Statie &s) {
        this->id=s.id;
        strcpy(this->nume,s.nume);
    }
    Statie& operator=(const Statie &s) {
        if (this != &s) {
            this->id = s.id;
            strcpy(this->nume, s.nume);
        }
        return *this;
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
class Magistrala { //basically clasa vector, dar magistrala
    private:
    Statie *statii;
    int n, max_size;
    char nume[50];
    void moreData() {
        Statie *temp= new Statie[max_size * 2];
        for (int i = 0; i < max_size; i++) {
            temp[i] = statii[i];
        }
        delete []statii;
        statii = temp;
        max_size*=2;
    }
    public:
    Magistrala(const char nume[]) {
        n = 0;
        max_size = 5;
        strcpy( this->nume , nume);
        statii=new Statie[max_size];
    }
    ~Magistrala() {
        delete[] statii;
        statii = NULL;
    }
    void adaugareStatie(const char nume[], int id) {
        if (n<max_size) {
            moreData();
        }
          Statie temp(id,nume);
            statii[n]=temp;
            n++;
    }
    void afisStatii() {
        for (int i = 0; i < n; i++) {
            statii[i].print();
        }
    }
    Statie getStatie(int id) {
        for (int i = 0; i < n; i++) {
            if (statii[i].getId()==id) {
                return statii[i];
            }
        }
        return statii[0];
    }
    void delStatie(int id) {
        for (int i = 0; i < n; i++) {
            if (statii[i].getId()==id) {
                for (int j = i; j < n; j++) {
                    statii[j] = statii[j+1];
                    --n;
                }
            }
        }
    }
    void loadStatii() {
        ifstream f("statii_metrou.txt");
        while (f >> id) {
            f>>nume;
            adaugareStatie(nume,id);
        }
        f.clear();
        f.close();
    }
    void saveStatii() {
        ofstream f("statii_metrou.txt");
        for (int i = 0; i <n; i++) {
            statii[i].printFile(f);
        }
        f.close();
    }
};
int main() {
    Magistrala M2("M2");
    M2.loadStatii();
    cin>>op;
    M2.saveStatii();
    return 0;
}