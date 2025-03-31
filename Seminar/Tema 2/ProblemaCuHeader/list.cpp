#include "list.h"
using namespace std;
List::List() {
    size = 0;
    ma = -999999999;//oricum verific ptr primul element si il pun pe max
}
List::List(const int x) {
    size = 1;
    ma = x;
    data.push_back(x);
}
List::List(const int x, const int no) {
    size = no;
    ma = x;
    for (int i = 0; i < size; i++) {
        data.push_back(x);
    }
}
List::List(const List& l) {
    size = l.size;
    ma = l.ma;
    for (int i = 0; i < l.size; i++) {
        data.push_back(l.data[i]);
    }
    //sau data=l.data
}
List::~List() {
    size = 0;
    //nu am ce sa distrug ca stl urile se gestioneaza singure
}
void List::addInt(int x) {
    if (size == 0) {
        data.push_back(x);
        size++;
        ma = x;
    } else {
        data.push_back(x);
        size++;
        if (x > ma) ma = x;
    }
}
void List::addInt(int x, int i) {
    if (size == 0) {
        addInt(x);
        return;
    }
    if (i > size) {
        cout << "index out of range" << endl;
        return;
    }
    if (x > ma) {
        ma = x;
    }
    data.push_back(0); //in caz ca ajung la capatul memoriei
    for (int j = size; j > i; j--) {
        data[j] = data[j - 1];
    }
    data[i] = x;
    size++;
}
int List::getSize() {
    return size;
}
int List::getMax() {
    return ma;
}
int List::getEl(const int i) {
    if (i >= size) {
        cout << "index out of range" << " ";
        return -1;
    }
    return data[i];
}
int List::operator[](const int i) {
    if (i >= size) {
        cout << "index out of range" << " ";
        return -1;
    }
    return data[i];
}
void List::print() {
    for (int i = 0; i < size; i++) {
        cout << data[i] << " ";
    }
}
void List::delByValue(const int value) {
    int i;
    for (i = 0; i < size; i++) {
        if (data[i] == value) {
            if (data[i] == ma) {
                for (int j = i; j < size - 1; j++) { //recalculam maximul doar daca acesta e indexul sters
                    data[j] = data[j + 1];//chiar daca era de 2 ori in vector, n am de unde sa stiu asta asa ca e safe sa fac iar calculul
                }
                size--;
                ma = data[0];
                for (int i = 0; i < size; i++) {
                    if (data[i] > ma) ma = data[i];
                }
            } else {
                for (int j = i; j < size - 1; j++) { //daca data[index] nici macar nu era maximul, nu are sens sa il recalculez
                    data[j] = data[j + 1];
                }
                size--;
            }
            return;
        }
    }
    if (i == size)
        cout << "value not found" << endl;
}
void List::delByIndex(const int index) {
    if (index >= size) {
        cout << "index out of range" << endl;
        return;
    }
    if (ma == data[index]) {
        for (int i = index; i < size - 1; i++) {//recalculam maximul doar daca acesta e indexul sters
            data[i] = data[i + 1];//chiar daca era de 2 ori in vector, n am de unde sa stiu asta asa ca e safe sa fac iar calculul
        }
        size--;
        ma = data[0];
        for (int i = 0; i < size; i++) {
            if (data[i] > ma) ma = data[i];
        }
    } else {
        for (int i = index; i < size - 1; i++) { //daca data[index] nici macar nu era maximul, nu are sens sa il recalculez
            data[i] = data[i + 1];
        }
        size--;
    }
}
