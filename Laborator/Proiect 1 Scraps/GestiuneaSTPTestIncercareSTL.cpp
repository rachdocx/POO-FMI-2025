#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
//refactored to use STL vector instead of custom implementation
//gestiunea sistemului de transport public proiect poo
using namespace std;
int op, id;
char nume_magistrala[50], nume_statie[50], nume_tren[50];
atomic<bool> stopLoop(false);
void checkForExit() {
    string input;
    while (true) {
        cin >> input;
        if (input == "exit") {
            stopLoop = true;
            break;
        }
    }
}

class Statie {
private:
    int id;
    char nume[50];
public:
    Statie() {
        id = 0;
        strcpy(nume, "");
    }
    Statie(int id, const char nume[]) {
        this->id = id;
        strcpy(this->nume, nume);
    }
    Statie(const Statie &s) {
        this->id = s.id;
        strcpy(this->nume, s.nume);
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
    char* getNume() {
        return nume;
    }
    void print() {
        cout<<this->nume<<".";
    }
    void printFile(ofstream &f) {
        f << this->id << " " << this->nume << endl;
    }
    friend istream &operator>>(istream &os, Statie &s1) {
        os >> s1.id >> s1.nume;
        return os;
    }
};

class Magistrala {
private:
    vector<Statie> statii;
    char nume_magistrala[50];

public:
    Magistrala() {
        strcpy(nume_magistrala, "");
    }

    Magistrala(const char nume[]) {
        strcpy(this->nume_magistrala, nume);
    }

    // No destructor needed as vector manages its own memory
    //de ce nu am un for aici?
    Magistrala& operator=(const Magistrala &other) {
        if (this != &other) {
            statii = other.statii;
            strcpy(nume_magistrala, other.nume_magistrala);
        }
        return *this;
    }

    char* getNume() {
        return nume_magistrala;
    }

    int getNr() {
        return statii.size();
    }

    void adaugareStatie(const char nume[], int id) {
        Statie temp1(id, nume);
        statii.push_back(temp1);
    }

    void adaugareStatie1(const Statie &s) {
        statii.push_back(s);
    }

    void adaugareStatieBack(const char nume[], int id) {
        Statie temp2(id, nume);
        statii.insert(statii.begin(), temp2);
    }

    void delStatieFront() {
        if (!statii.empty()) {
            statii.pop_back();
        }
    }

    void afisStatii() {
        for (size_t i = 0; i < statii.size(); i++) {
            statii[i].print();
            cout << endl;
        }
    }

    void afisStatie(int i) {
        if (i >= 0 && i < statii.size()) {
            statii[i].print();
        }
    }

    Statie getStatie(int id) {
        for (size_t i = 0; i < statii.size(); i++) {
            if (statii[i].getId() == id) {
                return statii[i];
            }
        }
        return statii.empty() ? Statie() : statii[0];
    }

    void delStatie(int id) {
        for (size_t i = 0; i < statii.size(); i++) {
            if (statii[i].getId() == id) {
                statii.erase(statii.begin() + i);
                break;
            }
        }
    }

    void saveStatii(ofstream &f) {
        for (size_t i = 0; i < statii.size(); i++) {
            f << statii[i].getId() << " " << statii[i].getNume() << endl;
        }
    }

    friend istream &operator>>(istream &os1, Magistrala &m) {
        int n;
        os1 >> n >> m.nume_magistrala;

        m.statii.clear();
        for (int i = 0; i < n; i++) {
            Statie temp;
            os1 >> temp;
            m.statii.push_back(temp);
        }
        return os1;
    }
};

class Sistem {
private:
    vector<Magistrala> magistrale;
    char nume_sistem[50];

public:
    Sistem() {
        strcpy(nume_sistem, "");
    }

    Sistem(const char nume[]) {
        strcpy(this->nume_sistem, nume);
    }

    void adaugareMagistrala(const Magistrala &ob, int nr_statii) {
        magistrale.push_back(ob);
    }

    void loadMagistrale(Sistem &s) {
        ifstream f("statii_metrou.txt");
        f >> s;
        f.clear();
        f.close();
    }
    friend istream &operator>>(istream &os1, Sistem &s) {
      Magistrala tempMagistrala;
      while (os1 >> tempMagistrala) {
        s.adaugareMagistrala(tempMagistrala, 0);
      }
      return os1;
    }

    void saveMagistrale() {
        ofstream f("statii_metrou.txt");
        for (size_t i = 0; i < magistrale.size(); i++) {
            f << magistrale[i].getNr() << ' ' << magistrale[i].getNume() << endl;
            magistrale[i].saveStatii(f);
        }
    }

    void afisMagistrale() {
        for (size_t i = 0; i < magistrale.size(); i++) {
            cout << magistrale[i].getNr() << " " << magistrale[i].getNume() << endl;
            magistrale[i].afisStatii();
            cout << endl;
        }
    }

    Magistrala* getMagistrala(const char nume[]) {
        for (size_t i = 0; i < magistrale.size(); i++) {
            if (strcmp(magistrale[i].getNume(), nume) == 0) {
                return &magistrale[i];
            }
        }
        return nullptr;
    }

    void deleteMagistralaByname(const char nume[]) {
        for (size_t i = 0; i < magistrale.size(); i++) {
            if (strcmp(magistrale[i].getNume(), nume) == 0) {
                magistrale.erase(magistrale.begin() + i);
                return;
            }
        }
    }
};

class Tren {
private:
    int id, cap_maxima;
    char nume[50], num_magistrala[50];
    float viteza_medie;
public:
    Tren() {
        strcpy(num_magistrala, "");
        cap_maxima = 0;
        id = 0;
        viteza_medie = 0;
        strcpy(nume, "");
    }

    Tren(int id, int cap_maxima, const char *nume, const char *num_magistrala, float viteza_medie) {
        this->id = id;
        this->cap_maxima = cap_maxima;
        strcpy(this->nume, nume);
        strcpy(this->num_magistrala, num_magistrala);
        this->viteza_medie = viteza_medie;
    }

    void setTren(Sistem &sistem) {
        Magistrala *temp = sistem.getMagistrala(this->num_magistrala);
        if (!temp) {
            cout << "Magistrala nu a fost gasita!" << endl;
            return;
        }

        thread inputThread(checkForExit);
        while (!stopLoop) {
            cout << "Trenul cu directia: ";
            temp->afisStatie(temp->getNr() - 1);
            cout << " pleaca de la:" << endl;
            for (int i = 0; i < temp->getNr(); i++) {
                temp->afisStatie(i);
                cout << endl;
                sleep(3);
                cout << "Trenul a ajuns la:" << endl;
                if (stopLoop) {
                    inputThread.join();
                    return;
                }
            }
            cout << "Trenul intoarce la: ";
            temp->afisStatie(temp->getNr() - 1);
            cout << " si pleaca catre: ";
            temp->afisStatie(0);
            cout << endl;
            cout << "Trenul cu directia: ";
            temp->afisStatie(0);
            cout << " pleaca de la:" << endl;
            for (int i = temp->getNr() - 1; i > 0; i--) {
                temp->afisStatie(i);
                cout << endl;
                sleep(3);
                cout << "Trenul a ajuns la:" << endl;
                if (stopLoop) {
                    inputThread.join();
                    return;
                }
            }
            temp->afisStatie(0);
            cout << endl;
        }
        stopLoop = false;
        inputThread.join();
    }

    void schimbareMagistrala(const char nume[]) {
        strcpy(num_magistrala, nume);
    }

    void saveTren() {
        ofstream f("trenuri.txt");
        f << id << ' ' << cap_maxima << ' ' << this->nume << ' ' << num_magistrala << ' ' << viteza_medie << endl;
    }

    char* getNume() {
        return nume;
    }

    friend istream& operator>>(istream& in, Tren& t) {
        in >> t.id;
        in >> t.nume;
        in >> t.num_magistrala;
        in >> t.cap_maxima;
        in >> t.viteza_medie;
        return in;
    }

    friend ostream& operator<<(ostream& out, const Tren& t) {
        out << t.id << " " << t.nume << " " << t.num_magistrala << " " << t.cap_maxima << " " << t.viteza_medie << " ";
        return out;
    }
};

class Depou {
private:
    char nume[50];
    vector<Tren> trenuri;

public:
    Depou() {
        strcpy(nume, "");
    }

    Depou(const char nume1[]) {
        strcpy(nume, nume1);
    }

    // No destructor needed as vector manages its own memory

    void loadTrenuri() {
        ifstream f("trenuri.txt");
        Tren temp;
        while (f >> temp) {
            trenuri.push_back(temp);
        }
        f.close();
    }

    void afisTrenuri() {
        for (size_t i = 0; i < trenuri.size(); i++) {
            cout << trenuri[i] << endl;
        }
    }

    void saveTrenuri() {
        ofstream f("trenuri.txt");
        for (size_t i = 0; i < trenuri.size(); i++) {
            f << trenuri[i] << endl;
        }
        f.close();
    }

    void addTrenuri(Tren &trenuri1) {
        trenuri.push_back(trenuri1);
    }

    Tren getTren(char nume1[]) {
        for (size_t i = 0; i < trenuri.size(); i++) {
            if (strcmp(nume1, trenuri[i].getNume()) == 0) {
                return trenuri[i];
            }
        }
        return Tren(); // Return default train if not found
    }

    Tren* getTren1(const char nume[]) {
        for (size_t i = 0; i < trenuri.size(); i++) {
            if (strcmp(nume, trenuri[i].getNume()) == 0) {
                return &trenuri[i];
            }
        }
        return nullptr;
    }

    void schimbareMagistrala(const char nume[], const char numeMag[]) {
        for (size_t i = 0; i < trenuri.size(); i++) {
            if (strcmp(nume, trenuri[i].getNume()) == 0) {
                trenuri[i].schimbareMagistrala(numeMag);
                return;
            }
        }
    }

    void deleteTrenByName(const char nume[]) {
        for (size_t i = 0; i < trenuri.size(); i++) {
            if (strcmp(nume, trenuri[i].getNume()) == 0) {
                trenuri.erase(trenuri.begin() + i);
                return;
            }
        }
    }
};

int main() {
    Sistem metrorex;
    Depou trenuriMetrorex;
    trenuriMetrorex.loadTrenuri();
    metrorex.loadMagistrale(metrorex);
    int optiune;
    cout << "1. Pentru Gestiunea Magistralelor" << endl;
    cout << "2. Pentru Gestiunea Trenurilor" << endl;
    cout << "0. Pentru iesirea din aplicatie" << endl;
    cin >> optiune;
    while (optiune != 0) {
        switch (optiune) {
            case 1:
                cout << "1 Pentru adaugare statie" << endl;
                cout << "2. Pentru adaugare magistrala" << endl;
                cout << "3. Pentru afisarea sistemului" << endl;
                cout << "4. Pentru a sterge o magistrala" << endl;
                cout << "5. Pentru a sterge o statie" << endl;
                cout << "0. Pentru a te intoarce" << endl;
                cin >> op;
                while (op != 0) {
                    switch (op) {
                        case 1: {
                            cout << "Introduceti numele magistralei unde vreti sa introduceti statia" << endl;
                            cin >> nume_magistrala;
                            cout << "Introduceti numele statiei" << endl;
                            cin >> nume_statie;
                            cout << "Introduceti ID-ul statiei" << endl;
                            cin >> id;
                            int op1;
                            cout << "1 Pentru push_front, 2 Pentru push_back" << endl;
                            cin >> op1;
                            Magistrala *m = metrorex.getMagistrala(nume_magistrala);
                            if (op1 == 1) {
                                if (m)
                                    m->adaugareStatie(nume_statie, id);
                                else
                                    cout << "Magistrala nu a fost gasita" << endl;
                            } else if (op1 == 2) {
                                if (m)
                                    m->adaugareStatieBack(nume_statie, id);
                                else
                                    cout << "Magistrala nu a fost gasita" << endl;
                            }
                        }
                        break;
                        case 2: {
                            cout << "Introduceti numele magistralei" << endl;
                            cin >> nume_magistrala;
                            Magistrala temp2(nume_magistrala);
                            metrorex.adaugareMagistrala(temp2, 0);
                        }
                        break;
                        case 3: {
                            metrorex.afisMagistrale();
                            break;
                        }
                        case 4: {
                            cout << "Introduceti numele magistralei de sters!" << endl;
                            cin >> nume_magistrala;
                            metrorex.deleteMagistralaByname(nume_magistrala);
                            break;
                        }
                        case 5: {
                            cout << "Atentie! Stergerea statiilor se face de la capetele magistralei" << endl;
                            cout << "Introduceti numele magistralei unde se afla statia de sters" << endl;
                            cin >> nume_magistrala;
                            cout << "Introduceti numele statiei de sters" << endl;
                            cin >> nume_statie;
                            cout << "1 Pentru pop_front, 2 Pentru pop_back" << endl;
                            int op3;
                            cin >> op3;
                            Magistrala *m = metrorex.getMagistrala(nume_magistrala);
                            if (op3 == 1 && m) {
                                m->delStatieFront();
                            }
                        }
                    }
                    cout << "1. Pentru adaugare statie" << endl;
                    cout << "2. Pentru adaugare magistrala" << endl;
                    cout << "3. Pentru afisarea sistemului" << endl;
                    cout << "4. Pentru a sterge o magistrala" << endl;
                    cout << "5. Pentru a sterge o statie" << endl;
                    cout << "0. Pentru a te intoarce" << endl;
                    cin >> op;
                }
                break;
            case 2:
                cout << "1. Afisarea tuturor trenurilor" << endl;
                cout << "2. Vizualizarea traseului unui tren" << endl;
                cout << "3. Pentru adaugarea unui tren" << endl;
                cout << "4. Pentru a muta trenul pe alta magistrala" << endl;
                cout << "5. Pentru a sterge un tren" << endl;
                cout << "0. Pentru a te intoare" << endl;
                cin >> op;
                while (op != 0) {
                    switch (op) {
                        case 1:
                            trenuriMetrorex.afisTrenuri();
                            break;
                        case 2: {
                            cout << "Introduceti numele trenului" << endl;
                            cin >> nume_tren;
                            Tren temp = trenuriMetrorex.getTren(nume_tren);
                            stopLoop = false;
                            temp.setTren(metrorex);
                            break;
                        }
                        case 3: {
                            cout << "Introduceti ID-ul, Numele, Magistrala, Capacitatea si Viteza trenului" << endl;
                            Tren temp1;
                            cin >> temp1;
                            trenuriMetrorex.addTrenuri(temp1);
                            break;
                        }
                        case 4: {
                            cout << "Introduceti numele trenului care trebuie mutat" << endl;
                            cin >> nume_tren;
                            cout << "Introduceti magistrala unde trebuie mutat" << endl;
                            cin >> nume_magistrala;
                            trenuriMetrorex.schimbareMagistrala(nume_tren, nume_magistrala);
                            break;
                        }
                        case 5: {
                            cout << "Introdu numele trenului:" << endl;
                            cin >> nume_tren;
                            trenuriMetrorex.deleteTrenByName(nume_tren);
                            break;
                        }
                    }
                    cout << "1. Afisarea tuturor trenurilor" << endl;
                    cout << "2. Vizualizarea traseului unui tren" << endl;
                    cout << "3. Pentru adaugarea unui tren" << endl;
                    cout << "4. Pentru a muta trenul pe alta magistrala" << endl;
                    cout << "5. Pentru a sterge un tren" << endl;
                    cout << "0. Pentru a te intoarce" << endl;
                    cin >> op;
                }
                break;
        }
        cout << "1. Pentru Gestiunea Magistralelor" << endl;
        cout << "2. Pentru Gestiunea Trenurilor" << endl;
        cout << "0. Pentru iesirea din aplicatie" << endl;
        cin >> optiune;
    }
    metrorex.saveMagistrale();
    trenuriMetrorex.saveTrenuri();
    return 0;
}