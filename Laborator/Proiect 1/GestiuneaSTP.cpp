#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
//gestiunea sistemului de transport public proiect poo
//de implementat clasa tren care parcuge statiile dintr o magistrala
//de adaugat in clasa statie timpul care dureaza de la statia precedenta pana la statia respectiva
//si sa simulez o parcurgere a trenului reala
//de facut friend functions pt input si output
using namespace std;
int op, id;
char nume[50], nume_magistrala[50], nume_statie[50];
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
    char *getNume() {
      return nume;
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
    char nume_magistrala[50];
    void moreData() {
        Statie *temp= new Statie[max_size * 2];
        for (int i = 0; i < max_size; i++) {
            temp[i] = statii[i];
        }
        if(statii!=NULL)
            delete []statii;
        statii = temp;
        max_size*=2;
    }
    public:
     Magistrala() {
         max_size=5;
       statii = new Statie[max_size];
       n=0;
       strcpy(nume_magistrala,"");
     }
    Magistrala(const char nume[]) {
        n = 0;
        max_size = 5;
        strcpy( this->nume_magistrala , nume);
        statii=new Statie[max_size];
    }
    ~Magistrala() {
        delete[] statii;
        statii = NULL;
    }
    Magistrala& operator=(const Magistrala &other) {
         if (this != &other) {
             delete[] statii;
             n = other.n;
             max_size = other.max_size;
             strcpy(nume_magistrala, other.nume_magistrala);
             statii = new Statie[max_size];
             for (int i = 0; i < n; i++) {
                 statii[i] = other.statii[i];
             }
         }
         return *this;
     }
    char *getNume() {
      return nume_magistrala;
    }
    int getNr(){
      return n;
      }
    void adaugareStatie(const char nume[], int id) {
        if (n>=max_size) {
            moreData();
        }
          Statie temp1(id,nume);
            statii[n]=temp1;
            n++;
    }
    void adaugareStatieBack(const char nume[], int id) {
      if (n>=max_size) {
        moreData();
      }
      Statie temp2(id,nume);
      for (int i = n; i > 0; i--) {
        statii[i] = statii[i-1];
      }
      statii[0] = temp2;
    }
    void afisStatii() {
        for (int i = 0; i < n; i++) {
            statii[i].print();
        }
    }
    void afisStatie(int i) {
      statii[i].print();
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
        char temp[50];
        f>>temp;
        strcpy(nume_magistrala,temp);
        while (f >> id) {
            f>>nume;
            adaugareStatie(nume,id);
        }
        f.clear();
        f.close();
    }
    void saveStatii(ofstream &f) {
        for (int i = 0; i <n; i++) {
            f<<statii[i].getId()<<" "<<statii[i].getNume()<<endl;
        }
    }
};
class Sistem{
  private:
    Magistrala *magistrale;
    int n, max_size;
    char nume_sistem[50];
    void moreData() {
      Magistrala *temp= new Magistrala[max_size * 2];
      for (int i = 0; i < max_size; i++) {
        temp[i] = magistrale[i];
      }
      delete [] magistrale;
      magistrale = temp;
      max_size*=2;
    }
   public:
     Sistem() {
       max_size=5;
       magistrale = new Magistrala[max_size];
       strcpy(nume_sistem,"");
       n=0;
     }
     Sistem(const char nume[]) {
       n = 0;
       max_size = 5;
       strcpy( this->nume_sistem, nume);
       magistrale=new Magistrala[max_size];
     }
     ~Sistem() {
       delete[] magistrale;
       magistrale = NULL;
     }
     void adaugareMagistrala(const Magistrala &ob, int nr_statii) {
       if (n>=max_size) {
           moreData();
       }
       magistrale[n]=ob;
       ++n;
     }
     void loadMagistrale(){
       ifstream f("statii_metrou.txt");
       int nr_statii;
       char temp_nume_magistrala[50], temp_nume_statie[50];
       while(f>> nr_statii) {
         f>>temp_nume_magistrala;
         Magistrala temp(temp_nume_magistrala);
         for(int i = 0; i < nr_statii; i++) {
           f>>id>>temp_nume_statie;
           temp.adaugareStatie(temp_nume_statie, id);
         }
        adaugareMagistrala(temp,nr_statii);
       }
       f.clear();
       f.close();
     }
     void saveMagistrale(){
       ofstream f("statii_metrou.txt");
       for (int i = 0; i <n; i++) {
         f<<magistrale[i].getNr()<<' '<<magistrale[i].getNume()<<endl;
         magistrale[i].saveStatii(f);
       }
     }
     void afisMagistrale(){
       for (int i = 0; i < n; i++) {
         cout<<magistrale[i].getNr()<<" "<<magistrale[i].getNume()<<endl;
         magistrale[i].afisStatii();
       }
     }
    Magistrala* getMagistrala(const char nume[]) {
         for (int i = 0; i < n; i++) {
             if (strcmp(magistrale[i].getNume(), nume) == 0) {
                 return &magistrale[i];
             }
         }
         return nullptr;
     }
};
class Tren{
private:
    int id, cap_maxima;
    char nume[50], num_magistrala[50];
    float viteza_medie;
public:
    Tren(){
        strcpy(num_magistrala, "");
        cap_maxima = 0;
        id = 0;
        viteza_medie = 0;
        strcpy(nume, "");
    }
    Tren(int id, int cap_maxima, const char *nume, const char *num_magistrala, float viteza_medie){
        this->id = id;
        this->cap_maxima = cap_maxima;
        strcpy(this->nume, nume);
        strcpy(this->num_magistrala, num_magistrala);
        this->viteza_medie = viteza_medie;
        strcpy(this->nume, "");
    }
    void setTren(Sistem &sistem){
      	Magistrala *temp = sistem.getMagistrala(this->num_magistrala);
        cout<<"Trenulcu directia Dimitrie Leonida pleaca de la:"<<endl;
		for (int i = 0; i < temp->getNr(); i++) {
        	temp->afisStatie(i);
            sleep(5);
            cout<<"Trenul a ajuns la:"<<endl;
		}
        }
};
int main() {
    Sistem metrorex("Metrorex");
    metrorex.loadMagistrale();
    Tren viena(1,50,"vienna","M2:",1);
    viena.setTren(metrorex);
    cout<<"1. Adaugare Statie!"<<endl;
    cout<<"2. Adaugare Magistrala!"<<endl;
    cout<<"3. Afisare Sistem!"<<endl;
    cin>>op;
    int op1;
    while (op!=0) {
      switch (op) {
        case 1:{
          cout<<"Introduceti numele magistralei unde introduceti statia"<<endl;
          cin>>nume_magistrala;
          cout<<"Introduceti numele statiei si ID-UL"<<endl;
          cin>>nume_statie>>id;
          cout<<"1 Pentru push_back, 2 Pentru push_front"<<endl;
          cin>>op1;
          Magistrala* m = metrorex.getMagistrala(nume_magistrala);
          if(op1==1){
              if (m) {
                  m->adaugareStatie(nume_statie, id);
              } else {
                  cout << "Magistrala nu a fost gasita!" << endl;
              }
          }
          else{
              if (m) {
                  m->adaugareStatieBack(nume_statie, id);
              } else {
                  cout << "Magistrala nu a fost gasita!" << endl;
              }
            }
         }
          break;
          case 2: {
              cout << "Introduceti numele magistralei: ";
              cin >> nume_magistrala;
              Magistrala temp2(nume_magistrala);
              metrorex.adaugareMagistrala(temp2, 0);
              break;
          }
         case 3:
             metrorex.afisMagistrale();
           break;
      }
        cout<<"1. Adaugare Statie!"<<endl;
        cout<<"2. Adaugare Magistrala!"<<endl;
        cout<<"3. Afisare Sistem!"<<endl;
     cin>>op;
    }
    metrorex.saveMagistrale();
    metrorex.afisMagistrale();
    return 0;
}