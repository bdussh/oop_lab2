#include <iostream>
#include "longint.h"

int main() {
    longint a;
    longint number1("4111");
    longint number2("60017001");
    longint number3 = Karatsuba(number1, number2);
    cout << endl;
    cout<<number3;
    return 0;
}
