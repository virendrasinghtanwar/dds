#include <iostream>
using namespace std;
void  printval(int num){
    if(num<=5){
        cout << num << "\n";
        printval(num+1);
    }
    else{
        return ;
    }
}
int main() {
    
    printval(1);

}
