#include <iostream>
#include "list.h"

using namespace std;
//am incercat sa fac problema mai eficienta, salvand maximul "global"
//si calculandu l doar atunci cand e neovie

//era interesant sa folosesc o structura de tip heap
//pentru a obtine maximul in O(1)
//dar din pacare conteaza ordinea elementelor
int main() {
    List li;
    cout<<"1. Test adding:"<<endl;
    li.addInt(1, 0); // adauga pe o pozitie
    li.addInt(2);     // adauga la dreapta
    li.addInt(3);
    li.addInt(4,4);
    li.addInt(4,3);
    li.addInt(5);
    li.addInt(6);
    li.addInt(7,7);
    li.addInt(7,6);
    li.print();
    cout<<endl;
    cout<<"max: "<<li.getMax()<<endl;
    cout<<endl;
    cout<<"2. Testing deleting and deleting the biggest interger: "<<endl;
    li.delByIndex(2);
    li.delByValue(4);
    li.delByIndex(4);
    cout<<"Max: "<<li.getMax()<<endl;
    li.delByValue(6);
    cout<<"Max: "<<li.getMax()<<endl;
    li.delByValue(7);
    li.delByIndex(7);
    li.print();
    cout<<endl;
    cout<<"Max: "<<li.getMax()<<endl;

    return 0;
}
