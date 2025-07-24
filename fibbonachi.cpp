#include <iostream>
using namespace std;
   
   int fib(int num){
       if (num==0){
           return 0;
       }
       if(num==1) {
      return 1;
       }
         else {
             return fib(num-1)+fib(num-2);
         }  
       
   }
int main() {
    int num ;
    cout << "ener the number for fibboanachi series ::";
     cin >> num;
     for (int i=0; i<num;i++){
         cout << fib(i)<<" ";
     }

    return 0;
}
