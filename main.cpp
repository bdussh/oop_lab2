#include <iostream>
#include "longint.h"

int main() {
    longint number1(237123);
    longint number2("2354");
    longint number3 = number1 * number2;
    cout << number3;
    cout << endl;
    return 0;
}
