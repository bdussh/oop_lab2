#include <vector>
#include <cmath>
#include <iostream>

#ifndef LAB2_LONGINT_H
#define LAB2_LONGINT_H

using namespace std;

class longint  {
private:


public:

    vector<int> digits;
    int digit_len = 1; //if not decimal system set as 1; because you will get into some problems, believe me!!!
    int base = 10;
    int digit_base = (int) pow(base, digit_len);

    longint(const char *number);

    longint(int number = 0);

    explicit longint(const string &number);

    friend ostream &operator<<(ostream &out, const longint &number_0);

    longint operator+(const longint &number);

    longint operator-(longint &number);

    bool operator>(const longint &number);

    friend bool operator<(longint number1, const longint &number2);

    bool operator==(const longint &number);

    longint operator*(const longint &number);

    longint operator*(int number);

    friend longint Karatsuba(longint x, longint y);

    friend longint ToomCook(longint x, longint y);

    friend longint power(longint x, int power);

};

vector<longint> split3(longint x, int n);
#endif //LAB2_LONGINT_H
