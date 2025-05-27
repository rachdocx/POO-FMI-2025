#include <iostream>
using namespace std;
class Clasa {
private:
public:


    friend ostream & operator << (ostream & os, Clasa & obj) {

        return os;
    }
    friend istream & operator >> (istream & is, Clasa & obj) {

        return is;
    }
};
class Masca {
private:
    string tip,culoare;
    int nr_pliuri;
public:
    Masca(string culoare, int nr_pliuri, string tip);
    const string getTip() const {
        return tip;
    }
};
Masca::Masca( string culoare, int nr_pliuri, string tip ="") {
    this->tip = tip;
    this->culoare = culoare;
    this->nr_pliuri = nr_pliuri;
}
class MascaPoli:public Masca {
private:
    string tip_prindere;

    const string tip;
public:
    MascaPoli(string culoare , int nr_pliuri, string tip_prindere);
    // const string getTip() const {
    //     return tip;
    // }
    // void setTip() {
    //     tip = "ff0";
    // }
};
MascaPoli::MascaPoli(string culoare, int nr_pliuri, string tip_prindere) : Masca(culoare,nr_pliuri),tip("tsdasd"){

}

int main() {
    Masca m1("alb", 5, "ff1");
    MascaPoli m2("verde", 9 , "elastic");
    cout<<m1.getTip();
    return 0;
}
