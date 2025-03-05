#include <iostream>
using namespace std;
class Stiva{
    struct nod{
      int x;
      nod* next;
    };
    nod* varf;
    public:
      Stiva(){
        varf = NULL;
      }

      ~Stiva(){
        if(varf){
          delete varf;
        }
      }

      void push(int x){
        nod* temp = new nod({x,varf});
        varf = temp;
      }
      int pop() {
        nod* temp = varf;
        varf = varf->next;
        int valoare = temp->x;
        delete temp;
        return valoare;
      }
      int varfStiva() {
        return varf->x;
      }
      int lungimeStiva() {
        nod *temp = varf;
        int nr=0;
        while(temp) {
          nr++;
          temp = temp->next;
        }
        return nr;
      }
      void afis() {
        if (varf == NULL) {
          cout << "stiva goala";
          return;
        }
        nod* temp = varf;
        while (temp) {
          cout << temp->x << " ";
          temp = temp->next;
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
  s.push(78);
  s.push(100);
  s.afis();
  cout << s.lungimeStiva() << endl;
}
