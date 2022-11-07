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
        result.digits.push_back(0);
    } else {
        int over = 0;
        for (int i = 0; i < size; ++i) {
            int operand1, operand2;

            operand1 = digits[i] - over;
            operand2 = number.digits.size() <= i ? 0 : number.digits[i];

            if (operand1 < operand2) {
                result.digits.push_back(((int) pow(base, digit_len) + operand1 - over) - operand2);
                over = 1;
            } else {
                result.digits.push_back(operand1 - operand2);
                over = 0;
            }
        }
    }

    if (result.digits[size - 1] == 0) {
        result.digits.pop_back();
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
    if (reminder) result.digits.push_back(reminder);

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

longint shift(longint n, int length){
    vector<int> insert;
    for(int i =0; i<length; i++)
        insert.push_back(0);
    n.digits.insert(n.digits.begin(),  insert.begin(), insert.end());
    return n;
}

pair<longint, longint> split(longint x, int n) {
    longint firstHalf;
    longint secondHalf;
    int i = 0;
    for (; i <= n/2; i++) {
        if(x.digits.size() <= i){
            firstHalf.digits.push_back(0);
        }
        else{
            firstHalf.digits.push_back(x.digits[i]);
        }
    }
    for(; i<n;i++){
        if(x.digits.size() <= i){
            secondHalf.digits.push_back(0);
        }
        else{
            secondHalf.digits.push_back(x.digits[i]);
        }
    }
    return pair(firstHalf, secondHalf);
}



longint Karatsuba(longint x, longint y) {
    if(x.digits.size() <= 1 || y.digits.size() <=1){
        return x*y;
    }
    int n = max(x.digits.size(), y.digits.size());
    pair<longint, longint> pair1 = split(x, n);
    pair<longint, longint> pair2 = split(y, n);

    longint a = pair1.first;
    longint b = pair1.second;
    longint c = pair2.first;
    longint d = pair2.second;

//    longint c_0 = Karatsuba(a,c);
//    longint c_1 = Karatsuba(b,d);
//    longint c_2 = Karatsuba(a+b,c+d) - c_0 - c_1;

    longint c_0 = a*c;
    longint c_1 = b*d;
    longint c_2 = (a+b)*(c+d) - c_0 - c_1;
    longint e = shift(c_1, (n + 1) / 2) + c_2;

    return shift(e, (n + 1) / 2) + c_0;
}


