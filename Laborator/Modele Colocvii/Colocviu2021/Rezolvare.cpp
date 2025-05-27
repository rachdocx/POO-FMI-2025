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
};
class Malware {
private:
    Data * data;
    float impact;
    string nume;
    string metoda;
    vector<string> registrii;
public:
    float getImpact(){
        return impact;
    }
    void inc_rating(float cat) {
        impact+=cat;
    }
    void inm_rating(float cat) {
        impact*=cat;
    }
    Malware(Data * data = nullptr, float impact = 0, string nume="N/A", string metoda ="N/A", vector<string> registrii ={}) {
        this->data=data;
        this->impact=impact;
        this->nume=nume;
        this->metoda=metoda;
        this->registrii=registrii;
    }
    virtual ~Malware() {
        if (data != nullptr)
            delete data;
    }
    friend ostream & operator << (ostream & os, Malware & obj) {

        os<<*obj.data<<endl<<obj.impact<<" "<<obj.metoda<<" "<<obj.nume<<endl;
        for (auto i:obj.registrii)
            os<<i<<" ";
        return os;
    }
    friend istream & operator >> (istream & is, Malware & obj) {
        if (obj.data == nullptr)
            obj.data = new Data();
        is>>*obj.data;
        cout<<"\nIntroduceti impactul: ";
        is>>obj.impact;
        cout<<"\nIntroduceti numele: ";
        is>>obj.nume;
        cout<<"\nIntroduceti metoda: ";
        is>>obj.metoda;
        cout<<"\nIntroduceti cati registrii: ";
        int n;
        is>>n;
        for (int i=0; i<n; ++i) {
            string temp;
            is>>temp;
            if (temp =="HKLM-run" || temp =="HKCU-run")
                obj.inc_rating(30);
            obj.registrii.push_back(temp);
        }
        return is;
    }
};
class Rootkit:virtual public Malware {
protected:
    vector<string> importuri;
    vector<string> strsemnificative;
public:
    Rootkit(Data * data = nullptr, float impact = 0, string nume="N/A", string metoda ="N/A", vector<string> registrii ={}, vector<string> importuri ={}, vector<string> strsemnificative={}):Malware(data,impact,nume,metoda,registrii) {
        this->importuri=importuri;
        this->strsemnificative=strsemnificative;
    }
    ~Rootkit() = default;
    friend ostream & operator << (ostream & os, Rootkit & obj) {
        os<<static_cast<Malware&>(obj)<<endl;
        for (auto i:obj.importuri)
            os<<i<<" ";
        os<<endl;
        for (auto i:obj.strsemnificative)
            os<<i<<" ";
        return os;
    }
    friend istream & operator >> (istream & is, Rootkit & obj) {
        is>>static_cast<Malware&>(obj);
        cout<<endl;
        int n;
        cout<<"Introdueti cate string-uri seminificative: ";
        is>>n;
        for (int i=1;i<=n;++i) {
            string temp;

            is>>temp;
            if (temp == "System_Service_Descriptor_Table" || temp =="SSDT" || temp == "NtCreateFile")
                obj.inc_rating(10);
            obj.strsemnificative.push_back(temp);
        }
        cout<<"Introduceti cate importuri: ";

        is>>n;
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            if (temp == "ntoskrnl.exe")
                obj.inm_rating(2);
            obj.importuri.push_back(temp);
        }
        return is;
    }

};
class Keylogger:virtual public Malware {
protected:
    vector<string> keys;
    vector<string> functii;
public:
    Keylogger(Data * data = nullptr, float impact = 0, string nume="N/A", string metoda ="N/A", vector<string> registrii ={}, vector<string> keys ={}, vector<string> functii={}):Malware(data,impact,nume,metoda,registrii) {
        this->keys=keys;
        this->functii=functii;
    }
    ~Keylogger() = default;
    friend ostream & operator << (ostream & os, Keylogger & obj) {
        os<<static_cast<Malware&>(obj)<<endl;
        for (auto i:obj.keys)
            os<<i<<" ";
        os<<endl;
        for (auto i:obj.functii)
            os<<i<<" ";
        return os;
    }
    friend istream & operator >> (istream & is, Keylogger & obj) {
        is>>static_cast<Malware&>(obj);
        cout<<endl;
        cout<<"Introduceti cate comenzi: ";
        int n;
        is>>n;
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            if (temp =="[Num Lock]" || temp =="[Down]" || temp == "[Right]" || temp == "[UP]" || temp == "[Left]"||temp=="[PageDown]")
                obj.inc_rating(10);

            obj.keys.push_back(temp);
        }
        cout<<"Introdueti cate functii: ";
        is>>n;
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            if (temp =="CreateFileW" || temp =="OpenProcess" || temp == "ReadFile" || temp == "WriteFile" || temp == "RegisterHotKey"||temp=="SetWindowsHookEx")
                obj.inc_rating(10);
            obj.functii.push_back(temp);
        }
        return is;
    }
};
class KernelKeylogger: public Keylogger, public Rootkit{
    bool is_hiding;
public:
    KernelKeylogger(Data * data = nullptr, float impact = 0, string nume="N/A", string metoda ="N/A", vector<string> registrii ={}, vector<string> keys ={}, vector<string> functii={}, vector<string> importuri ={}, vector<string> strsemnificative={},bool is_hiding=false):Keylogger(data,impact,nume,metoda,registrii,keys,functii),Rootkit(data,impact,nume,metoda,registrii,importuri,strsemnificative) {
        this->is_hiding=is_hiding;
    }
    ~KernelKeylogger() =default;
    friend ostream & operator << (ostream & os, KernelKeylogger & obj) {
        os<<static_cast<Rootkit&>(obj)<<endl;
        for (auto i:obj.keys)
            os<<i<<" ";
        cout<<endl;
        for (auto i:obj.functii)
            os<<i<<" ";
        cout<<endl;
        cout<<obj.is_hiding;
        return os;
    }
    friend istream & operator >> (istream & is, KernelKeylogger & obj) {
        is>>static_cast<Rootkit&>(obj);
        cout<<endl;
        cout<<"Introduceti cate comenzi: ";
        int n;
        is>>n;
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            if (temp =="[Num Lock]" || temp =="[Down]" || temp == "[Right]" || temp == "[UP]" || temp == "[Left]"||temp=="[PageDown]")
                obj.inc_rating(10);
            obj.keys.push_back(temp);
        }
        cout<<"Introdueti cate functii: ";
        is>>n;
        for (int i=1;i<=n;++i) {
            string temp;
            is>>temp;
            if (temp =="CreateFileW" || temp =="OpenProcess" || temp == "ReadFile" || temp == "WriteFile" || temp == "RegisterHotKey"||temp=="SetWindowsHookEx")
                obj.inc_rating(10);
            obj.functii.push_back(temp);
        }
        cout<<endl;
        cout<<"Introduceti daca ascunde fisiere";
        is>>obj.is_hiding;
        if (obj.is_hiding)
            obj.inc_rating(30);
        return is;
    }
};
class Ransomware:public Malware {
private:
    float rat_crip, rat_ob;
public:
    Ransomware(Data * data = nullptr, float impact = 0, string nume="N/A", string metoda ="N/A", vector<string> registrii ={}, float rat_crip =0, float rat_ob =0):Malware(data,impact,nume,metoda,registrii) {
        this->rat_crip=rat_crip;
        this->rat_ob=rat_ob;
    }
    ~Ransomware()=default;
    friend ostream & operator << (ostream & os, Ransomware & obj) {
        os<<static_cast<Malware&>(obj)<<endl;
        os<<obj.rat_crip<<" "<<obj.rat_ob<<endl;
        return os;
    }
    friend istream & operator >> (istream & is, Ransomware & obj) {
        is>>static_cast<Malware&>(obj);
        cout<<"\nIntroduceti rating ul de criptare: ";
        is>>obj.rat_crip;
        cout<<"\nIntroduceti rating ul de ob: ";
        is>>obj.rat_ob;
        obj.inc_rating(obj.rat_crip);
        obj.inc_rating(obj.rat_ob);
        return is;
    }
};
class Computer {
    vector<Malware*> virusi;
    const int id_calculator;

    static int id_generator;
public:
    Computer(vector<Malware*> virusi ={}):id_calculator(id_generator++) {
        this->virusi=virusi;
    }
    ~Computer() {
        for (auto i:virusi)
            delete i;
        virusi.clear();
    }
    vector<Malware *> *getMalware() {
        return &virusi;
    }
    const int getId() {
        return id_calculator;
    }
};
class Menu {
private:
    Menu()=default;
    Menu(const Menu &)=delete;
    Menu & operator=(const Menu&) = delete;
    inline static Menu * MENU;
    vector<Computer*> calculatoare;
public:
    static Menu * getInstance() {
        if (MENU == nullptr)
            MENU = new Menu();
        return MENU;
    }
    void afisInfo() {
        for (auto i:calculatoare) {
            vector<Malware*>* p = i->getMalware();
            cout<<"Calculatorul cu id-ul "<<i->getId() <<"este:\n";
            for (auto j: *p) {
                if (Rootkit * temp = dynamic_cast<Rootkit*>(j))
                    cout<<*temp;
                if (Keylogger * temp = dynamic_cast<Keylogger*>(j))
                    cout<<*temp;
                if (KernelKeylogger * temp = dynamic_cast<KernelKeylogger *>(j))
                    cout<<*temp;
                if (Ransomware * temp = dynamic_cast<Ransomware *>(j))
                    cout<<*temp;
            }
        }
    }
    void afisInfoDupaOrdonare() {

    }
    void afisInfoKDupaOrdonare(int k) {

    }
    void afisProcent() {
        int nr_inf=0;
        int nr_curat=0;
        for (auto i:calculatoare) {
            bool inf=false;
            vector<Malware*>* p = i->getMalware();
            for (auto j:*p) {
                if (j->getImpact() != 0)
                    inf = true;
            }
            if (inf==0)
                nr_curat++;
            else
                nr_inf++;
        }
        cout<<nr_curat<<" "<<nr_inf;
    }
};
int main() {
    Menu *s = Menu::getInstance();
    int op=-1;
    do {
        cout<<"1. Pentru a afisa toate calculatoarele\n";
        cout<<"2. Pentru a le afisa ordonat dupa impact\n";
        cout<<"3. Pentru a le afisa pe primele k dupa impact\n";
        cout<<"4. Pentru a afisa raportul de cal infectate\n";
        cin>>op;
        try {
            if (op<0 || op >4)
                throw logic_error("Optiunea nu este valida!\n");
            switch (op) {
                case 1: {
                    s->afisInfo();
                    break;
                }
                case 2: {
                    s->afisInfoDupaOrdonare();
                    break;
                }
                case 3: {
                    cout<<"Cititi cate k: ";
                    int k;
                    cin>>k;
                    s->afisInfoKDupaOrdonare(k);
                    break;
                }
                case 4: {
                    s->afisProcent();
                    break;
                }
            }
        }catch (logic_error & e) {
            cout<<e.what();
        }
    }while (op!=0);
    return 0;
}
