#include <iostream>
using namespace std;
class Stiva{
    struct Nod{
      int x;
      Nod* next;
    };
    Nod* varf;
    public:
      Stiva(){
        varf = NULL;
      }

      ~Stiva(){
        if(varf != NULL){
          delete varf;
        }
      }

      void push(int x){
        Nod* temp = new Nod({x,varf});
        varf = temp;
      }
      int pop() {
        Nod* temp = varf;
        varf = varf->next;
        int valoare = temp->x;
        delete temp;
        return valoare;
      }
      void afis() {
        if (varf == NULL) {
          cout << "stiva goala";
          return;
        }
        Nod* temp = varf;
        while (temp) {
          cout << temp->x << " ";
          temp = temp->next; //
        }
        cout << endl;
      }

};
int main() {
  Stiva s;
  s.push(5);
  s.push(6);
  s.push(7);
  s.push(7);
  s.push(8);
  s.push(9);
  s.pop();
  cout << s.pop() << endl;
  s.afis();
}
