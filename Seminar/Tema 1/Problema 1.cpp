#include <iostream>
using namespace std;

class Stiva {
  class Nod {  // Definim clasa Nod în loc de struct
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
  // Constructor implicit
  Stiva() {
    varf = nullptr;
  }

  // Constructor cu parametru
  Stiva(int x) {
    varf = new Nod(x, nullptr);
  }

  // Constructor de copiere
  Stiva(const Stiva& s) {
    varf = nullptr;
    Nod* temp = s.varf;
    while (temp) {
      push(temp->x);
      temp = temp->next;
    }
  }

  // Destructor
  ~Stiva() {
    while (varf != nullptr) {
      Nod* temp = varf;
      varf = varf->next;
      delete temp;
    }
  }

  void push(int x) {
    Nod* temp = new Nod(x, varf);
    varf = temp;
  }

  int pop() {
    if (varf != nullptr) {
      Nod* temp = varf;
      varf = varf->next;
      int valoare = temp->x;
      delete temp;
      return valoare;
    } else {
      cout << "Stiva este deja goala" << endl;
      return -1;
    }
  }

  int varfStiva() {
    return (varf != nullptr) ? varf->x : -1;
  }

  int lungimeStiva() {
    int nr = 0;
    Nod* temp = varf;
    while (temp) {
      nr++;
      temp = temp->next;
    }
    return nr;
  }

  void afis() {
    if (varf == nullptr) {
      cout << "Stiva goala" << endl;
      return;
    }
    Nod* temp = varf;
    while (temp) {
      cout << temp->x << " ";
      temp = temp->next;
    }
    cout << endl;
  }

  bool is_empty() {
    return varf == nullptr;
  }
};

int main() {
  Stiva s(1);
  s.push(2);
  s.push(3);
  Stiva s2(s);
  s2.push(7);
  s2.push(8);
  s2.afis();
  s.afis();
}
