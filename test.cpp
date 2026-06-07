#include <iostream>

using namespace std;

void sumToA(int &a, int b) { a += b; }

int main() {
    int a = 5;
    sumToA(a, 10);
    cout << "The sum is: " << a << endl;    
    
    return 0;
}