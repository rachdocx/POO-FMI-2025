#include <iostream>
using namespace std;
class Vector {
    int *data;
    int n;
    int max_size;
    void more_data() {
        int *temp = new int[max_size + 50];
        for (int i=0; i<n; ++i) {
            temp[i]=data[i];
        }
        delete[] data;
        data = temp;
        max_size+=50;
    }
public:
    //constructor implicit
    Vector() {
        n=0;
        max_size=50;
        data = new int[max_size];
    }
    //constructor cu un singur parametru, max_size
    Vector(int max_size1) {
        n==0;
        max_size=max_size1;
        data = new int[max_size];
    }
    //constructor care primeste un array si il baga in vector
    Vector(int n1, int max_size1, int v[]={}) {
        n=n1;
        max_size=max_size1;
        data = new int[max_size];
        for (int i=0; i<n; ++i) {
            data[i]=v[i];
        }
    }
    //constructor care initializeaza n1 elemente egale cu c
    Vector(int n1, int max_size1, int c) {
        n=n1;
        max_size=max_size1;
        data = new int[max_size];
        for (int i=0; i<n; ++i) {
            data[i]=c;
        }
    }
    //constructor de copiere
    Vector(const Vector& v) {
        n=v.n;
        max_size=v.max_size;
        data = new int[max_size];
        for (int i=0; i<n; ++i) {
            data[i]=v.data[i];
        }

    }
    ~Vector() {
        delete[] data;
        data = NULL;
    }
    void add(int x) {
        if (n<max_size) {
            data[n]=x;
            ++n;

        }
        else {
            more_data();
            data[n]=x;
            ++n;
        }
    }
    int last_el() {
        if (n==0) {
            cout<<"vectorul este gol"<<endl;
            return -1;
        }else
            return data[n];
    }
    void del() {
        if (n==0) {
            cout<<"vectorul este gol"<<endl;
            return;
        }else {
            data[n]=0;
            --n;
        }
    }
    void print() {
        if (data!=NULL) {
            for (int i=0; i<n; ++i) {
                cout<<data[i]<<" ";
            }
            cout<<endl;
        }
    }
    int print_size() {
        cout<<n<<endl;
        return n;
    }
    int acc_el(int i) {
        if (i>=0 && i<=n)
            return data[i];
        else {
            cout<<"index invalid"<<endl;
            return -1;
        }
    }
};
int main(){
  Vector v;
    v.add(7);
    v.add(10);
    for (int i=0; i<60; i++) {
        v.add(i);
    }
    v.del();
    cout<<v.acc_el(7)<<endl;
    v.print();
    v.print_size();
    cout<<v.last_el()<<endl;
    int valori[]={1,2,3,4,5};
    Vector v2(5,50,valori);
    v2.print();
    Vector v3(5,50,10);
    v3.print();
    Vector v4(v3);
    v4.print();
    v4.add(100);
    v4.print();
  return 0;
  }