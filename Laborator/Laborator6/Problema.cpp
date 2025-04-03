#include <iostream>
using namespace std;

class A {
protected:
    int i;
};

class B : A {
public: // mare greseala!
    using A::i;
};

class C : public B {
public:
    void setI(int i) {
        this->i = i;
    }
};

class M {
protected:
    int x;
public:
    M(int x = 0) {
        cout << "Constructor M" << endl;
        this->x = x;
    }
    ~M() {
        cout << "Destructor M" << endl;
    }
};
// pune static la x in clasa to see what happens
//int M::x = 1;

class M1 : public M {
protected:
    int i;
public:
    M1(int i = 0) {
        cout << "Constructor M1" << endl;
        this->i = i;
    }
    ~M1() {
        cout << "Destructor M1" << endl;
    }
};

class M2 : public M {
protected:
    int i;
public:
    M2(int i = 0) {
        cout << "Constructor M2" << endl;
        this->i = i;
    }
    ~M2() {
        cout << "Destructor M2" << endl;
    }
};

class MM : public M1, public M2 {
public:
    MM() {
        cout << "Constructor MM" << endl;
    }
    ~MM() {
        cout << "Destructor MM" << endl;
    }
    void ambigousI(int i = 0) {
        // Ce inseamna this->i? Este mostenit si din M1 si M2
        cout << &(this->M2::i) << ' ' << &(this->M1::i) << endl;
    }

    void ambigousX() {
        this->M1::x = 8;
        cout << this->M1::x << ' ' << this->M2::x << endl;

        cout << &(this->M1::x) << ' ' <<
                               &(this->M2::x) << endl;
    }
};

struct Datetime {
    unsigned int day, month, year, hour, minute, second;
};

class User {
protected:
    char *username, *password;
    Datetime date_of_join, date_of_birth;
    bool gender, isOnline;
public:
    User() {
        cout << "[USER] Constructor fara parametrii" << endl;

        this->username = new char[strlen("Anonim") + 1];
        strcpy(this->username, "Anonim");

        this->password = new char[strlen("changeme") + 1];
        strcpy(this->password, "changeme");

        date_of_birth = {1, 1, 1970, 0, 0, 0};
        date_of_join = {1, 1, 1970, 0, 0, 0};

        gender = false;
        isOnline = false;
    }

    User(char* username,
         const char* password = "changeme",
         Datetime date_of_join = {1, 1, 1970, 0, 0, 0},
         Datetime date_of_birth = {1, 1, 1970, 0, 0, 0},
         bool gender = false,
         bool isOnline = false) {
        cout << "[USER] Constructor cu unul sau mai multi parametrii" << endl;
        this->gender = gender;
        this->isOnline = isOnline;

        this->date_of_join = date_of_join;
        this->date_of_birth = date_of_birth;

        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);

        this->username = new char[strlen(username) + 1];
        strcpy(this->username, username);
    }

    char* getUsername() const {
        return this->username;
    }

    void sendMessage(char* message) {
        cout << "[MESAJ - USER]: " << message << endl;
    }
};

class Admin : public User {
private:
    char *message_of_the_day;
public:
    Admin() : User("John Doe") {
        cout << "[ADMIN] Constructor fara parametrii" << endl;

        this->message_of_the_day = new char[strlen("None") + 1];
        strcpy(this->message_of_the_day, "None");
    }

    void setUsername(char* username) {
        if (this->username != NULL) {
            delete[] this->username;
        }

        this->username = new char[strlen(username) + 1];
        strcpy(this->username, username);
    }

    char* getUsername() const {
        cout << "[ADMIN] Get username apelat" << endl;
        return this->username;
    }

    void sendMessage() {
        cout << "[MESAJ - ADMIN]: Acesta este un mesaj implicit" << endl;
    }
};

int main() {
    Admin admin;

    cout << admin.getUsername() << endl;
    admin.setUsername("Lorem Ipsum");
    cout << admin.getUsername() << endl;

    admin.sendMessage();

    MM mm;
//    mm.ambigousI();

    mm.ambigousX();
    return 0;
}