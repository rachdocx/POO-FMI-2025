#include <iostream>
using namespace std;
class List{
   vector<int> data;
   int size, ma;
   public:
   List(){
     size = 0;
     ma = -999999999; //oricum verific ptr primul element si il pun pe
   }
   ~List(){
     size = 0;
   }
   List(const List& l){
     size = l.size;
     ma = l.ma;
     data = l.data;
   }
   void addInt(int x){
     if(size == 0){
       data.push_back(x);
       size++;
       ma = x;
     }
     else{
       data.push_back(x);
       size++;
       if(x > ma)
         ma = x;
     }
   }
   void addInt(int x, int i){
      if(size == 0){
       addInt(x);
       return;
     }
     if(i >= size-1){
       cout<<"index out of range"<<endl;
       return;
     }
     if(x > ma){
       ma = x;
     }
     for(int j = size; j > i; j--){
       data[j] = data[j-1];
     }
     data[i] = x;
     size++;
   }
   int getSize(){
     return size;
   }
   int getMax(){
     return ma;
   }
   //getteri ptr elementul de pe poz i
   int getEl(const int i){
     if(i >= size){
       cout<<"index out of range"<<" ";
       return -1;
     }
     return data[i];
   }
   int operator[](const int i){
     if(i >= size){
       cout<<"index out of range"<<" ";
       return -1;
     }
     return data[i];
   }
   //
   void print(){
     for(int i = 0; i < size; i++){
       cout<<data[i]<<" ";
     }
   }
   void delByValue(const int value){
     int i;
     for(i = 0; i < size; i++){
       if(data[i] == value){
         if(data[i] == ma){
           for(int j = i; j < size-1; j++){  //recalculam maximul doar daca acesta e indexul sters
           data[j] = data[j+1];            //chiar daca era de 2 ori in vector, n am de unde sa stiu asta asa ca e safe sa fac iar calculul
           }
           size--;
           ma = data[0];
           for(int i = 0; i < size; i++){
             if(data[i] > ma)
               ma = data[i];
           }
         }
         else
           {
              for(int j = i; j < size-1; j++){    //daca data[index] nici macar nu era maximul, nu are sens sa il recalculez
               data[j] = data[j+1];
               }
               size--;
           }
           return;
       }
     }
     if(i == size)
       cout<<"value not found"<<endl;
     return;
   }
   void delByIndex(const int index){
     if(index >= size){
       cout<<"index out of range"<<endl;
       return;
     }
     if(ma == data[index]){
       for(int i = index; i < size-1; i++){  //recalculam maximul doar daca acesta e indexul sters
           data[i] = data[i+1];            //chiar daca era de 2 ori in vector, n am de unde sa stiu asta asa ca e safe sa fac iar calculul
       }
       size--;
       ma = data[0];
       for(int i = 0; i < size; i++){
         if(data[i] > ma)
           ma = data[i];
       }
     }
     else{
       for(int i = index; i < size-1; i++){    //daca data[index] nici macar nu era maximul, nu are sens sa il recalculez
         data[i] = data[i+1];
       }
       size--;
     }
   }
};
int main(){
  List li;
  li.addInt(69, 0); //adauga pe o pozitie
  li.addInt(1); //adauga la dreapta
  li.addInt(2);
  li.delByValue(2);
  li.print();
  cout<<endl;
  cout<<li.getEl(1)<<" ";
  cout<<li[1]<<endl;
  cout<<li.getSize()<<" "<<li.getMax()<<endl;
}