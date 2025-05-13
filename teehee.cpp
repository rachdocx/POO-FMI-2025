#include <iostream>

int f() { return 1; }
const int g() { return 2; }

int main() {
     const int a = f();
     int b = g();
     std::cout << a + b;
}
