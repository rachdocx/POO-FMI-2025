#include <iostream>
using namespace std;
const unsigned int MAX_LENGTH = 256;

class Student {
private:
    const int studentId;
    char *fullname;
    int group;

    static int generatorId;
    static int szListOfStudents;
    static Student* listOfStudents;
public:
    Student(char *fullname = "Anonim", int group = -1) : studentId(generatorId++), group(group) {
        this->fullname = new char[strlen(fullname) + 1];
        strcpy(this->fullname, fullname);

        this->group = group;
    }

    Student(const Student& student) : studentId(generatorId++) {
        this->group = student.group;

        if (this->fullname != nullptr) {
            delete[] this->fullname;
        }
        this->fullname = new char[strlen(student.fullname) + 1];
        strcpy(this->fullname, student.fullname);
    }

    ~Student() {
        if (this->fullname != nullptr) {
            delete[] this->fullname;
        }
    }

    friend istream& operator>>(istream& in, Student& student) {
        cout << "Introdu numele studentului: ";
        in.get(student.fullname, MAX_LENGTH);

        cout << "Introdu grupa din care face parte studentul: ";
        in >> student.group;

        return in;
    }

    friend ostream& operator<<(ostream& out, const Student& student) {
        out << "ID STUDENT: " << student.studentId << endl;
        out << "Nume student: " << student.fullname << endl;
        out << "Grupa student: " << student.group << endl;

        return out;
    }

    Student& operator=(const Student& student) {
        this->group = student.group;

        if (this->fullname != nullptr) {
            delete[] this->fullname;
        }
        this->fullname = new char[strlen(student.fullname) + 1];
        strcpy(this->fullname, student.fullname);

        return *this;
    }

    static void addStudentToList(const Student& student) {
        listOfStudents[szListOfStudents++] = student;
    }

    static void showAllStudents() {
        for (int i = 0; i < szListOfStudents; i++) {
            cout << listOfStudents[i] << endl;
        }
    }

    static void showStudentById(int studentId) {
        for (int i = 0; i < szListOfStudents; i++) {
            if (listOfStudents[i].studentId == studentId) {
                cout << listOfStudents[i] << endl;
                return;
            }
        }

        cout << "Nu am gasit un student cu id-ul mentionat: " << studentId << endl;
    }

    static Student* getStudentById(int studentId) {
        for (int i = 0; i < szListOfStudents; i++) {
            if (listOfStudents[i].studentId == studentId) {
                return &listOfStudents[i];
            }
        }

        return nullptr;
    }

    // SETTERS & GETTERS
    void setGroup(int group) {
        this->group = group;
    }

    int getGroup() const {
        return this->group;
    }

    int getStudentId() const {
        return this->studentId;
    }

    void setFullname(char *fullname) {
        if (this->fullname != nullptr) {
            delete[] this->fullname;
        }
        this->fullname = new char[strlen(fullname) + 1];
        strcpy(this->fullname, fullname);
    }

    char* getFullname() const {
        return this->fullname;
    }

    static void deleteStudentById(int studentId) {
        bool startDelete = false;
        if (studentId == listOfStudents[szListOfStudents - 1].studentId) {
            szListOfStudents -= 1;
            return;
        }

        for (int i = 1; i < szListOfStudents; i++) {
            if (studentId == listOfStudents[i - 1].studentId) {
                startDelete = true;
            }
            if (startDelete) {
                listOfStudents[i - 1] = listOfStudents[i];
            }
        }

        if (startDelete) {
            szListOfStudents -= 1;
        } else {
            cout << "Nu am gasit studentul cu ID-ul mentionat: " << studentId << endl;
        }
    }

    static void deleteStudentList() {
        szListOfStudents = 0;
        if (listOfStudents != nullptr) {
            delete[] listOfStudents;
        }
    }

    // se face adunarea intre un student si un int
    // ex

    // Student s1; int a = 5;
    // s1 = s1 + a;
    // s1 = a + s1;

    // Versiunea cu friend a lui este
    friend Student operator+(Student& student, int x) {
        student.group += x;
        return student;
    }

    Student operator+(int x) {
        this->group += x;
        return *this;
    }

    friend Student operator+(int x, Student& student) {
        student.group += x;
        return student;
    }

    Student& operator+=(int x){
        this->group += x;
        return *this;
    }
};
// Atributele statice se init in afara clasei, niciodata in ea sau
// in constructor (deoarece ele sunt proprietati ce tin de clasa, nu de obiect)
int Student::generatorId = 1000;
int Student::szListOfStudents = 0;
Student* Student::listOfStudents = new Student[MAX_LENGTH];


int main() {
    char command[MAX_LENGTH];
    // Meniu interactiv
    do {
        cout << "[Gestiune Universitate] -- MENIU --: " << endl;
        cout << "1. Introdu un student in aplicatie" << endl;
        cout << "2. Vizualizeaza un student din aplicatie" << endl;
        cout << "3. Vizualizeaza toti studentii din aplicatie" << endl;
        cout << "4. Actualizeaza un student din aplicatie" << endl;
        cout << "5. Sterge un student din aplicatie" << endl;
        cout << "Pentru a opri aplicatia scrie stop." << endl;
        cout << "Optiunea ta este (1-5 sau stop): ";

        cin >> command;
        cin.get();

        if (!strcmp(command, "stop")) {
            cout << "Ai oprit programul de gestiune" << endl;
            break;
        }

        int numberCommand = atoi(command);
        switch(numberCommand) {
            case 1: {
                Student student;
                cin >> student;

                Student::addStudentToList(student);
                /*Student student1, student2;
                // listOfStudents trebuie sa fie acelasi
                // si pentru student1 si pentru student2 =>
                // devine atribut de clasa (se foloseste static)
                */

                /*
                 * a + b, +: A x B -> R, +(a, b)
                 * (cin >> a) >> b
                 *
                 * operator>>(cin, student) <=>
                 * cin >> student
                 * student.operator>>(cin) <=>
                 * student >> cin
                 */
                break;
            }
            case 2: {
                /*
                 * Student s1(1000, "Anonim", -1), s2(1001, "John Doe", 141);
                 * s1 = s2;
                 * s1 Student(1000, "John Doe", 141)
                 */
                int studentIdToView;
                cout << "Introdu un id al unui student pe care vrei sa il vizualizezi: ";
                cin >> studentIdToView;

                Student::showStudentById(studentIdToView);
                break;
            }
            case 3: {
                Student::showAllStudents();
                break;
            }
            case 4: {
                int studentIdToSearch;
                cout << "Introdu un id al unui student pe care vrei sa il actualizezi: ";
                cin >> studentIdToSearch;
                cin.get();

                Student* student = Student::getStudentById(studentIdToSearch);
                if (student != nullptr) {
                    // Metoda slow (nu o recomand) <=> PATCH
//                    int group; char fullname[MAX_LENGTH];
//                    cout << "Introdu numele: " << endl;
//                    cin.get(fullname, MAX_LENGTH);
//                    cin >> group;
//
//                    student->setGroup(group);
//                    student->setFullname(fullname);


                    // Metoda desteapta (poate o recomand)
                    Student studentAux;
                    cin >> studentAux;
                    *student = studentAux;

                } else {
                    cout << "Studentul nu exista cu id-ul mentionat: " << studentIdToSearch << endl;
                }
                break;

            }
            case 5: {
                int studentIdToSearch;
                cout << "Introdu un id al unui student pe care vrei sa il stergi: ";
                cin >> studentIdToSearch;

                Student::deleteStudentById(studentIdToSearch);
                break;
            }
            default:
                cout << "Ai introdus o optiune invalida" << endl;
        }
    } while(true);
    Student s3;
    int a = 4;
    cout << s3.getStudentId() << endl;

    s3 = s3 + a;
    s3 = a + s3;
//    s3 += a;
    cout << s3 << endl;

    Student::deleteStudentList();
    return 0;
}