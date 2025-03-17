#include <iostream>
using namespace std;
class Stiva{
  class Nod {
  public:
    int x;
    Nod* next;

    Nod(int val, Nod* nxt = nullptr) {
      x = val;
      next = nxt;
    }
  };
    Nod* varf;
    public:
      //implicit
      Stiva(){
        varf = NULL;
      }
      //cu parametrii
      Stiva(int x) {
        varf = new Nod(x,NULL);
      }
    //constructorul de copiere
      Stiva (Stiva& s) {     //const Stiva& s eventual
            varf = NULL;
            Nod* temp = s.varf;
            while (temp) {
              push(temp->x);
              temp = temp->next;
            }
      }
  Stiva& operator=(const Stiva& other) {
        if (this == &other)
          return *this;
        while (!is_empty()) {
          pop();
        }
        Stiva aux;
        Nod* temp = other.varf;
        while (temp) {
          aux.push(temp->x);
          temp = temp->next;
        }
        while (!aux.is_empty()) {
          push(aux.pop());
        }
        return *this;
      }

      ~Stiva(){
        while(varf != NULL) {
          Nod* temp = varf;  //sau s.pop()
          varf = varf->next;
          delete temp;
        }
      }

      void push(int x){
        Nod* temp = new Nod(x,varf);
        varf = temp;
      }

      int pop() {
        if (varf != NULL) {
          Nod* temp = varf;
          varf = varf->next;
          int valoare = temp->x;
          delete temp;
          return valoare;
        }
        else {
          cout<<"stiva este deja goala"<<endl;
          return -1;
        }
      }
      int varfStiva() {
        if (varf != NULL)
          return varf->x;
        else
          return -1;
      }

      int lungimeStiva() {
        if (varf == NULL)
          return 0;
        else {
          Nod *temp = varf;
          int nr=0;
          while(temp) {
            nr++;
            temp = temp->next;
          }
          return nr;
        }
      }
      void afis() {
        if (varf == NULL) {
          cout << "stiva goala"<<endl;
          return;
        }
        Nod* temp = varf;
        while (temp) {
          cout << temp->x << " ";
          temp = temp->next;
        }
        cout << endl;
      }

    int is_empty() {
        if (varf == NULL)
          return 1;
        else
          return 0;
      }

};
int main() {
  Stiva s;
  s.push(1);
  s.push(2);
  s.pop();
  s.push(3);
  Stiva s1(s);
  s1.push(4);
  s1.afis();
  s.afis();
  Stiva s2;
  s2 = s1;
  s2.afis();
  cout << s1.varfStiva() << s1.lungimeStiva() << endl;
  s1.pop();
  s1.pop();
  s1.pop();
  s1.pop();
  s1.pop();
  s1.pop();
  s1.is_empty();
  s1.afis();
}
