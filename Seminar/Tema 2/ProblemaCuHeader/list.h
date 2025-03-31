#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <vector>

class List {
private:
    std::vector<int> data;
    int size, ma;
public:
    List(); //constructor ptr o lista goala
    List(const int x);//constructor lista initializata cu un singur element
    List(const int x, const int no);//constructor lista initializata cu no elemente egale cu x
    List(const List& l);//constructor de copiere
    ~List(); //destructor
    void addInt(int x);//adaugare int la dreapta
    void addInt(int x, int i); //overload adaugare int la un index
    int getSize(); //getter size
    int getMax();//getter max
    int getEl(const int i);//getteri ptr elementul de pe poz i
    int operator[](const int i);//overload [] asemanator cu getEl ptr index
    void print();
    void delByValue(const int value);
    void delByIndex(const int index);
};

#endif
