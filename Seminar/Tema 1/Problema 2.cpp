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
    Vector() {
        n=0;
        max_size=50;
        data = new int[max_size];
    }
    ~Vector() {
        delete[] data;
        data = NULL;
        cout<<"oare merge?"<<endl;
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
  return 0;
  }