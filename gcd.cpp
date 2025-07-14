#include <iostream>
using namespace std;

int  gcdval(int a, int b){
    if(b==0){
        return a;
    }
    else{
       return  gcdval(b,a%b);
        
    }
    
    }

int main() {
    int a = 12;
    int b= 18;
    
   int c= gcdval(a,b);
   cout << c;
}
