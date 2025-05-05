# C++ Templates Laboratory Exercises

These exercises are designed to help you practice the concepts of C++ templates covered in the lab documentation and examples. Try to solve them step-by-step.

## Exercise 1: Simple Function Template

**Objective:** Write and use a basic function template.

**Task:**
1.  Create a function template named `swapValues` that takes two arguments of the same type `T` by reference.
2.  The function should swap the values of the two arguments.
3.  In your `main` function, demonstrate the `swapValues` template by:
    *   Declaring two integer variables, initializing them, printing their values, swapping them using `swapValues`, and printing their values again.
    *   Declaring two double variables, initializing them, printing their values, swapping them using `swapValues`, and printing their values again.
    *   Declaring two `std::string` variables, initializing them, printing their values, swapping them using `swapValues`, and printing their values again.

**Hint:** You might need a temporary variable of type `T` inside the `swapValues` function.

## Exercise 2: Simple Class Template (Generic Box)

**Objective:** Implement and use a basic class template.

**Task:**
1.  Create a class template named `Box` that can store a single item of any type `T`.
2.  The class should have:
    *   A private member variable of type `T` to store the item.
    *   A constructor that takes an argument of type `T` to initialize the item.
    *   A public member function `getItem()` that returns the stored item.
    *   A public member function `displayItem()` that prints the stored item to the console, preceded by a label (e.g., "Box contains: ").
3.  In your `main` function, demonstrate the `Box` template by:
    *   Creating a `Box` object to store an integer (e.g., `Box<int> intBox(123);`).
    *   Calling `displayItem()` on the integer box.
    *   Creating a `Box` object to store a string (e.g., `Box<std::string> stringBox("Template Lab");`).
    *   Calling `displayItem()` on the string box and also retrieving the item using `getItem()` and printing it separately.

## Exercise 3: Template with Multiple Type Parameters (Generic Pair)

**Objective:** Work with templates that have more than one type parameter.

**Task:**
1.  Modify the `Pair` class template from the examples (or create a new one) so that it can hold two values of potentially *different* types. Name the template parameters `T1` and `T2`.
2.  The class should have:
    *   Private member variables `first_element` of type `T1` and `second_element` of type `T2`.
    *   A constructor that takes two arguments, one of type `T1` and one of type `T2`, to initialize the members.
    *   Public member functions `getFirst()` returning `T1` and `getSecond()` returning `T2`.
    *   A public member function `display()` that prints the pair (e.g., "(value1, value2)").
3.  In `main`, create `Pair` objects with different type combinations (e.g., `Pair<int, double>`, `Pair<std::string, int>`) and display their contents.

## Exercise 4: Debugging a Template Error (Instantiation Error)

**Objective:** Identify and fix a common template instantiation error.

**Task:**
1.  Below is a piece of code containing an error similar to the instantiation error example. Copy this code into a new `.cpp` file.
2.  Compile the code. Observe the compilation error message. Understand why the error occurs.
3.  Fix the error by adding the necessary functionality to the `Widget` class so that the `printLarger` template function can work correctly with `Widget` objects.
4.  Recompile and run the code to verify the fix.

```cpp
#include <iostream>

class Widget {
private:
    int weight;
public:
    Widget(int w) : weight(w) {}
    int getWeight() const { return weight; }

    // Missing comparison operator!
    // Add the necessary operator overload here.
    // bool operator>(const Widget& other) const { ... }
};

// Template function to print the larger of two items
template <typename T>
void printLarger(T a, T b) {
    if (a > b) { // This line causes the error for Widget
        std::cout << "The first item is larger." << std::endl;
    } else {
        std::cout << "The second item is larger or they are equal." << std::endl;
    }
}

int main() {
    Widget w1(10);
    Widget w2(20);

    std::cout << "Comparing Widgets w1(10) and w2(20):" << std::endl;
    printLarger(w1, w2); // This call fails without the fix

    int i1 = 50, i2 = 30;
    std::cout << "\nComparing ints 50 and 30:" << std::endl;
    printLarger(i1, i2); // Works fine for int

    return 0;
}
```

## Exercise 5: (Optional) Template Specialization

**Objective:** Implement template specialization for a specific type.

**Task:**
1.  Start with the generic `Box` class template from Exercise 2.
2.  Create a full class template specialization for `Box<const char*>`. This specialized version should:
    *   Store the C-style string (`const char*`).
    *   Have a `displayItem()` function that prints the string enclosed in double quotes (e.g., `Box contains: "some string"`).
    *   (Optional bonus) Add a member function `getLength()` that returns the length of the stored string (use `std::strlen` from `<cstring>`).
3.  In `main`, create a `Box<int>` and a `Box<const char*>` and call `displayItem()` on both to show that the generic and specialized versions are being used correctly.

 