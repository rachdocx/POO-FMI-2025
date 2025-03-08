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
        while(varf != NULL) {
          nod* temp = varf;  //sau s.pop()
          varf = varf->next;
          delete temp;
        }
      }

      void push(int x){
        nod* temp = new nod({x,varf});
        varf = temp;
      }

      int pop() {
        if (varf != NULL) {
          nod* temp = varf;
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
        if (varf == NULL)
          return varf->x;
      }

      int lungimeStiva() {
        if (varf == NULL)
          return 0;
        else {
          nod *temp = varf;
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

    int is_empty() {
        if (varf == NULL)
          return 1;
        else
          return 0;
      }

};
int main() {
  Stiva s;
}
