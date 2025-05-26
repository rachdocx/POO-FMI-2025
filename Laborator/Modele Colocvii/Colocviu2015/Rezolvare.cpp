#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class Product {
    protected:
        string measure_unit;
        string name;
    public:
        Product(string measure_unit = "N/A", string name = "N/A") {
            this->measure_unit = measure_unit;
            this->name = name;
        }
        ~Product() = default;
        const string getType() {
            return "Product";
        };
        friend istream & operator >> (istream & is, Product & obj){
            is >> obj.measure_unit >> obj.name;
            return is;
        }
        friend ostream & operator << (ostream & os, Product & obj) {
            os << obj.measure_unit << obj.name;
            return os;
        }
};
class PerishableProduct: virtual public Product {
    protected:
        string expiration_date;
    public:
        PerishableProduct(string measure_unit = "N/A", string name = "N/A", string expiration_date = "N/A") : Product(measure_unit, name) {
            this->expiration_date = expiration_date;
        }
        ~PerishableProduct() = default;
        const string getType() {
            return "Perishable";
        }

};
class DiscountedProduct: virtual public Product {
    protected:
        double discount;
    public:
        DiscountedProduct(string measure_unit = "N/A", string name = "N/A", double discount = 0) : Product(measure_unit, name) {
            this->discount = discount;
        }
        ~DiscountedProduct() = default;
        const string getType() {
            return "Discounted";
        }
};
class DiscountedPerishableProduct:public DiscountedProduct, public PerishableProduct {
    public:
        DiscountedPerishableProduct(string measure_unit = "N/A", string name ="N/A", double discount = 0, string expiration_date = "N/A"):Product(measure_unit,name),DiscountedProduct(measure_unit, name, discount), PerishableProduct(measure_unit, name, expiration_date) {

        }
        ~DiscountedPerishableProduct() = default;
        const string getType() {
            return "DiscountedPerishable";
        }
};
class Batch {
    Product* produs;
    int price;
    int quantity;
    string date;
    public:
        Batch(Product * produs = nullptr, int price=0, string date = "N/A", int quantity = 0) {
            this->produs = produs;
            this->price = price;
            this->date = date;
            this->quantity = quantity;
        }
        ~Batch() {
            if (produs != nullptr)
                delete produs;
        }
};
string global_prod_type, global_date, global_mu, global_name, global_exp_date;
int  global_quantity, global_price;
int main() {
    ifstream f("input1.txt");
    vector < Batch * > loturi;
    while (f>>global_prod_type) {
        if (global_prod_type == "produs_perisabil") {
            f>>global_date>>global_quantity>>global_mu>>global_name>>global_exp_date>>global_price;
            PerishableProduct * temp = new PerishableProduct(global_mu, global_name, global_exp_date);
            loturi.push_back(new Batch(temp, global_price, global_date, global_quantity));
        }
    }
    return 0;
}
