#include <iostream>
using namespace std;
void  printval(int i,int n){
    if(i<=n){
        cout << i << "\n";
        printval(i+1,n);
    }
    else{
        return ;
    }
}
int main() {
    int n;
    int i=1;
    cin >> n;
    
    printval(i,n);

}
