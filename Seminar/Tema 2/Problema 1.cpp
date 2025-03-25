#include <iostream>
using namespace std;
class List{
   vector<int> data;
   int *no_of_ints, *ma; //vreau sa distrug si variabilele simple nu doar array ul, asa ca lucrez cu pointeri
   public:
     List(){
       no_of_ints = new int;
       ma = new int;
       *no_of_ints = 0;
       *ma = -999999999;
     }
     ~List(){
       delete no_of_ints;
       delete ma;
     }
     List(const List& l){
     }
     void addBack(int val){
         if(*no_of_ints == 0){
           (*no_of_ints)++;
           *ma = val;
         }
         data.push_back(val);
     }
     void addFront(int val){
       if(*no_of_ints == 0){
         (*no_of_ints)++;
         *ma = val;
       }
       data.insert(data.begin(), val);
     }
      void printList(){
       for(int i=0;i<data.size();i++){
         cout<<data[i]<<" ";
       }
     }
};
int main(){
  List li;
  li.addBack(10);
  li.addBack(20);
  li.addFront(30);
  li.printList();
}