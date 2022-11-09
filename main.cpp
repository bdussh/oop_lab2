#include <iostream>
#include "longint.h"

int main() {
    longint a;
    longint number1("721");
    longint number2("235");

    longint number3 = ToomCook(number1,  number2);
    cout << endl;
    cout << number3;
    return 0;
}
