#include <iostream>
#include <typeinfo>
#include <vector>
using namespace std;

class Base {
public:
    virtual ~Base() {}
};

class Derived : public Base {};

class Animal {
public:
    virtual void speak() { cout << "Animal sound" << endl; }
    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void speak() { cout << "Woof!" << endl; }
    void bark() { cout << "Bark bark" << endl; }
};

class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
};

/// In case I don't implement the draw method Circle rests as an Abstract Class
class Circle : public Shape {
public:
    void draw() override {
        cout << "Drawing Circle" << endl;
    }
};

class Square : public Shape {
public:
    void draw() override {
        cout << "Drawing Square" << endl;
    }
};

template<class T1=int, class T2=int>
struct custom_pair {
    T1 first;
    T2 second;
};

template<typename T=int>
class MyVector {
private:
    T* data; // dynamic array
    unsigned int sz; // size
    unsigned int cap; // capacity

    void resize_MyVector(unsigned int new_cap) {
        T* new_data = new T[new_cap];
        for (unsigned int i = 0; i < sz; i++) {
            new_data[i] = data[i];
        }

        if (data != NULL) {
            delete[] data;
        }
        data = new_data;
        cap = new_cap;
    }
public:
    MyVector() : data(nullptr), sz(0), cap(0) {}
    ~MyVector() {
        if (data != NULL) {
            delete[] data;
        }
    }

    void push_back(const T& value) {
        if (sz == cap) {
            this->resize_MyVector(cap == 0 ? 1 : cap * 2);
        }
        data[sz++] = value;
    }

    void pop_back() {
        if (sz > 0) sz--;
    }

    T& operator[](unsigned int index) {
        if (index >= sz) throw out_of_range("Index out of bounds.");
        return data[index];
    }

    unsigned int size() const {
        return sz;
    }

    unsigned int capacity() const {
        return cap;
    }
};

int main()
{
    /// Typeinfo -> typeid on primitive types
    int i = 42;
    double pi = 3.14;
    char c = 'A';

    cout << typeid(i).name() << endl;
    cout << typeid(pi).name() << endl;
    cout << typeid(c).name() << endl;

    /// TypeInfo - typeid on self-defined types (classes)
    Base *basePtr = new Derived();

    cout << "Type of object pointed by basePtr: " <<
        typeid(*basePtr).name() << endl;
    /// When using dereferencing values the typeid shows the TRUE type
    /// which is discoved by the RTTI (using the VTABLE).
    cout << "Type of basePtr itself: " << typeid(basePtr).name() << endl;
    /// When using the pointer it only checks the pointer type, not the object.

    Base *nullBasePtr = nullptr;
    try {
        typeid(*nullBasePtr);
    } catch (bad_typeid& error) {
        cout << "Caught exception: " << error.what() << endl;
    }

    ////////////////////////////////////////////////////////
    /**                  Dynamic Cast                    **/
    ////////////////////////////////////////////////////////
    Animal *pet = new Dog();
    // Animal *trickyPet = new Animal(); -> replace at line 60 with pet
    pet->speak(); // polymorphic call to speak method

    // Safe Downcasting part 1 -> with pointers
    Dog *dog = dynamic_cast<Dog*>(pet);
    if (dog != nullptr) {
        dog->bark();
    } else {
        cout << "The pet is not a dog." << endl;
    }

    /**
        Rule of thumb for dynamic_cast:
            - Only works with polymorphic classes (i.e. with at least one virtual method)
        In case it fails:
            - for pointers: returns nullptr (NULL)
            - for references: throws std::bad_cast (class bad_cast : public exception) (throws an exception)
    **/
    Dog trueDog;
    Animal &referencePet = trueDog;
    try {
        Dog &d = dynamic_cast<Dog&>(referencePet);
    } catch (bad_cast error) {
        cout << "[Caught an exception]: " << error.what() << endl;
    }

    //////////////////////////////////////////////
    // TypeInfo -> typeid in an example problem //
    //////////////////////////////////////////////

    // Polimorphic container
    vector<Shape*> shapes;

    shapes.push_back(new Circle());
    shapes.push_back(new Square());
    shapes.push_back(new Circle());

    for (Shape* shape : shapes) {
        shape->draw();

        if (typeid(*shape) == typeid(Circle)) {
            cout << "This is a circle" << endl;
        } else if (typeid(*shape) == typeid(Square)) {
            cout << "This is a square" << endl;
        }
    }

    /// Avoid it!
    /**
        - RTTI is slow compared to simple polymorphic virtual calls.
        - Prefer polymorphism and virtual functions instead of type checking everywhere.
    **/

    /// Welcome to STL!
    /// 1. Pair
    pair<int, int> p = {20, 30}; // initialization (list_initializer)
    cout << p.first << ' ' << p.second << endl;

    pair<int, int> q = make_pair(30, 40); // make_pair initialization
    cout << q.first << ' ' << q.second << endl;

    custom_pair<> cp = {30, 40};
    cout << cp.first << ' ' << cp.second << endl;

    /// 2. Vector (Implementation level vector (dynamic arrays contiguous memory) <-> stack <-> queue <-> list (double linked list))
    vector<int> d = {50, 40, 30, 20, 10};
    d.push_back(60);
    d.push_back(70);
    d.pop_back();
    cout << d.front() << ' ' << d.back() << endl;
    /// index based search
    for (unsigned int i = 0, j = d.size(); i < j; i++) {
        cout << d[i] << endl;
        // OR
        cout << d.at(i) << endl;
    }

    /// iterator
    // auto stands for vector<int>::iterator
    for (auto it = d.begin(); it != d.end(); it++) {
        cout << *it << endl;
    }

    /// foreach (like for ... in ... from python)
    // auto = int, auto = int& (auto takes by default the reference type)
    for (auto element : d) {
        cout << element << endl;
    }

    MyVector<> vec;
    vec.push_back(50);
    vec.push_back(40);
    vec.push_back(30);
    vec.push_back(20);
    vec.push_back(10);
    vec.pop_back();

    cout << vec.size() << ' ' << vec.capacity() << endl;
    for (unsigned int i = 0; i < vec.size(); i++) {
        cout << vec[i] << ' ';
    }
    return 0;
}
