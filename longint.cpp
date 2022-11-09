#include <cmath>
#include "longint.h"

int lenOfNum(int a) {
    int count = 0;
    while (a) {
        a /= 10;
        count++;
    }
    return count;
}

longint::longint(const char *number1) {
    string number = number1;
    for (int i = number.size() - 1; i >= 0; i -= digit_len) {
        int digit = 0;
        int shift = 1;
        for (int j = 0; j < digit_len && i - j >= 0; j++) {
            digit += (number[i - j] - '0') * shift;
            shift *= 10;
        }
        digits.push_back(digit);
    }

}

longint::longint(int number) {
    int some = (int) pow(base, digit_len);
    for (int i = lenOfNum(number); i > 0; i -= digit_len) {
        digits.push_back(number % (int) pow(base, digit_len));
        number /= some;
    }
}


longint::longint(const string &number) {
    for (int i = number.size() - 1; i >= 0; i -= digit_len) {
        int digit = 0;
        int shift = 1;
        for (int j = 0; j < digit_len && i - j >= 0; j++) {
            digit += (number[i - j] - '0') * shift;
            shift *= 10;
        }
        digits.push_back(digit);
    }
}


ostream &operator<<(ostream &out, const longint &number) {
    int length = number.digits.size();

    out << number.digits[length - 1];
    for (int i = length - 2; i >= 0; --i) {
        bool breaker = false;
        for (int j = 1; j < number.digit_len; j++) {
            if (!breaker && number.digits[i] < pow(number.base, j)) {
                for (int k = 0; number.digit_len - j - k > 0; k++) {
                    out << '0';
                }
                breaker = true;
            }
        }
        out << number.digits[i];
    }
    return out;
}

longint longint::operator+(const longint &number) {
    longint result("");
    int over = 0;

    for (int i = 0; i < max(digits.size(), number.digits.size()); ++i) {
        int operand1, operand2;

        operand1 = digits.size() <= i ? 0 : digits[i];
        operand2 = number.digits.size() <= i ? 0 : number.digits[i];

        result.digits.push_back((operand1 + operand2 + over) % (int) pow(base, digit_len));
        over = operand1 + operand2 + over >= (int) pow(base, digit_len) ? 1 : 0;
    }
    if (over == 1) result.digits.insert(result.digits.end(), 1);
    return result;
}

bool longint::operator==(const longint &number) {
    if (digits.size() != number.digits.size()) return false;
    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] != number.digits[i]) return false;
    }
    return true;
}

bool longint::operator>(const longint &number) {
    int size1 = digits.size();
    int size2 = number.digits.size();
    if (size1 > size2) return true;
    else if (size1 < size2) return false;
    for (int i = size1 - 1; i >= 0; --i) {
        if (digits[i] > number.digits[i]) return true;
        else if (digits[i] < number.digits[i]) return false;
    }
    return false;
}


longint longint::operator-(longint &number) {
    longint result("");
    int size = digits.size();
    if (!(number < *this)) {
        return longint(0);
    } else {
        int operand1, operand2;
        int overflow = 0;
        result.digits.push_back(-overflow);
        for (int i = 0; i < digits.size(); i++) {

            operand2 = number.digits.size() > i ? number.digits[i] : 0;
            result.digits[i] += digits[i] - operand2;
            if (result.digits[i] < 0) {
                result.digits[i] += pow(base, digit_len);
                overflow = 1;
            } else {
                overflow = 0;
            }
            result.digits.push_back(-overflow);
        }
        while (result.digits[result.digits.size() - 1] == 0) {
            result.digits.pop_back();
        }

    }
    return result;
}

bool operator<(longint number1, const longint &number2) {
    if (number1 > number2 || number1 == number2) {
        return false;
    } else {
        return true;
    }
}


longint longint::operator*(int number) {
    int digit_base1 = (int) pow(base, digit_len);
    longint result("");
    int reminder = 0;
    for (int i = 0; i < digits.size(); ++i) {
        result.digits.push_back(reminder);
        result.digits[i] += (digits[i] * number);
        reminder = result.digits[i] / digit_base1;
        result.digits[i] %= digit_base1;
    }
    while (reminder){
        int temp = reminder%digit_base;
        reminder = reminder/ digit_base;
        result.digits.push_back(temp);
    }
    return result;
}

longint power(longint x, int power) {
    longint result = 1;
    for (int i = 0; i < power; i++) {
        result = result * x;
    }
    return result;
}

longint longint::operator*(const longint &number) {
    vector<int> shift;
    longint result("0");
    longint additor("0");
    for (int digit: number.digits) {
        additor = *this * digit;
        additor.digits.insert(additor.digits.begin(), shift.begin(), shift.end());
        result = result + additor;
        shift.push_back(0);
    }
    return result;
}

longint shift(longint n, int length) {
    vector<int> insert;
    for (int i = 0; i < length; ++i)
        insert.push_back(0);
    n.digits.insert(n.digits.begin(), insert.begin(), insert.end());
    return n;
}

pair<longint, longint> split(longint x, int n) {
    longint firstHalf;
    longint secondHalf;
    int i = 0;
    for (; i < n / 2; i++) {
        if (x.digits.size() <= i) {
            firstHalf.digits.push_back(0);
        } else {
            firstHalf.digits.push_back(x.digits[i]);
        }

    }

    for (; i < n; i++) {
        if (x.digits.size() <= i) {
            secondHalf.digits.push_back(0);
        } else {
            secondHalf.digits.push_back(x.digits[i]);
        }

    }
    return pair(firstHalf, secondHalf);
}


longint Karatsuba(longint x, longint y) {
    if (x.digits.size() <= 1 || y.digits.size() <= 1) {
        return x * y;
    }
    int n = max(x.digits.size(), y.digits.size()) % 2 == 0 ? max(x.digits.size(), y.digits.size()) :
            max(x.digits.size(), y.digits.size()) + 1;
    pair<longint, longint> pair1 = split(x, n);
    pair<longint, longint> pair2 = split(y, n);

    longint a = pair1.first;
    longint b = pair1.second;
    longint c = pair2.first;
    longint d = pair2.second;

    longint c_0 = Karatsuba(a, c);
    longint c_1 = Karatsuba(b, d);
    longint c_2 = Karatsuba(a + b, c + d) - c_0 - c_1;
    longint e = shift(c_1, (n) / 2) + c_2;
    longint e1 = shift(e, (n) / 2) + c_0;

    return shift(e, (n) / 2) + c_0;
}

vector<longint> split3(longint x, int n) {
    vector<longint> result;
    int j = 0;
    for (int i = 1; i <= 3; i++) {
        longint temp;
        for (; j < n * i / 3; j++) {
            if (x.digits.size() <= j) {
                temp.digits.push_back(0);
            } else {
                temp.digits.push_back(x.digits[j]);
            }
        }
        result.push_back(temp);
    }
    return result;
}


longint ToomCook(longint x, longint y) {
    if (x.digits.size() <= 1 || y.digits.size() <= 1) {
        return x * y;
    }
    if (x.digits.size() <= 2 || y.digits.size() <= 2) {
        return Karatsuba(x, y);
    }
    int n = max(x.digits.size(), y.digits.size());
    if (n % 3 == 1) {
        n += 2;
    }
    if (n % 3 == 2) {
        n++;
    }

    vector<longint> temp1 = split3(x, n);
    vector<longint> temp2 = split3(y, n);

    longint x0 = temp1[0];
    longint x1 = temp1[1];
    longint x2 = temp1[2];

    longint y0 = temp2[0];
    longint y1 = temp2[1];
    longint y2 = temp2[2];


    longint a0 = x0;
    longint a1 = x0 + x1 + x2;
    longint a2 = x0 + x1 * 2 + x2 * 4;
    longint a3 = x0 + x1 * 3 + x2 * 9;
    longint a4 = x0 + x1 * 4 + x2 * 16;

    longint b0 = y0;
    longint b1 = y0 + y1 + y2;
    longint b2 = y0 + y1 * 2 + y2 * 4;
    longint b3 = y0 + y1 * 3 + y2 * 9;
    longint b4 = y0 + y1 * 4 + y2 * 16;

    longint c0 = a0 * b0;
    longint c1 = a1 * b1;
    longint c2 = a2 * b2;
    longint c3 = a3 * b3;
    longint c4 = a4 * b4;

    longint f0 = c0;
    longint f1 = c1 - f0;
    longint f2 = c2 - f1 - c1; // multiplied by 2
    longint f3 = (c3 + c1 - c2 - c2) - f2; // multiplied by 6
    longint f4 = (c4 + c2 + c2 + c2 - c3 - c3 - c3 - c1) - f3; // multiplied by 24

    f1 = f1 * x.digit_base;
    f2 = f2 * ((x.digit_base * (x.digit_base - 1)) / 2);
    f3 = f3 * ((x.digit_base * (x.digit_base - 1) * (x.digit_base - 2)) / 6);
    f4 = f4 * ((x.digit_base * (x.digit_base - 1) * (x.digit_base - 2) * (x.digit_base - 3)) / 24);
    longint result = f0 + f1 + f2 + f3 + f4;
    return result;
}




