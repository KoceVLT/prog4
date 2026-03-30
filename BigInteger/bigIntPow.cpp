#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class BigInt {
public:
    string value;

    BigInt() {
        value = "0";
    }

    BigInt(string s) {
        int i = 0;
        while (i < (int)s.length() - 1 && s[i] == '0') {
            i++;
        }
        value = s.substr(i);
    }

    bool isLess(string n1, string n2) const {
        if (n1.length() != n2.length()) {
            return n1.length() < n2.length();
        }
        return n1 < n2;
    }

    bool operator<(const BigInt& other) const {
        return isLess(value, other.value);
    }

    BigInt operator+(const BigInt& other) const {
        string s1 = value;
        string s2 = other.value;
        string res = "";
        int i = (int)s1.length() - 1;
        int j = (int)s2.length() - 1;
        int carry = 0;

        while (i >= 0 || j >= 0 || carry != 0) {
            int v1 = 0;
            if (i >= 0) {
                v1 = s1[i] - '0';
                i = i - 1;
            }
            int v2 = 0;
            if (j >= 0) {
                v2 = s2[j] - '0';
                j = j - 1;
            }
            int sum = v1 + v2 + carry;
            res.push_back((char)((sum % 10) + '0'));
            carry = sum / 10;
        }
        reverse(res.begin(), res.end());

        int pos = 0;
        while (pos < (int)res.length() - 1 && res[pos] == '0') {
            pos++;
        }
        res = res.substr(pos);
        return BigInt(res);
    }

    BigInt operator-(const BigInt& other) const {
        if (*this < other) {
            return BigInt("0");
        }

        string s1 = value;
        string s2 = other.value;
        string res = "";
        int i = (int)s1.length() - 1;
        int j = (int)s2.length() - 1;
        int borrow = 0;

        while (i >= 0) {
            int v1 = s1[i] - '0';
            int v2 = 0;
            if (j >= 0) {
                v2 = (s2[j] - '0');
                j = j - 1;
            }
            v2 = v2 + borrow;

            if (v1 < v2) {
                v1 = v1 + 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            res.push_back((char)((v1 - v2) + '0'));
            i = i - 1;
        }
        while (res.length() > 1 && res.back() == '0') {
            res.pop_back();
        }
        reverse(res.begin(), res.end());
        return BigInt(res);
    }

    BigInt operator*(const BigInt& other) const {
        int n = (int)value.length();
        int m = (int)other.value.length();
        vector<int> temp;
        for (int k = 0; k < n + m; k = k + 1) {
            temp.push_back(0);
        }

        for (int i = n - 1; i >= 0; i = i - 1) {
            for (int j = m - 1; j >= 0; j = j - 1) {
                int mul = (value[i] - '0') * (other.value[j] - '0');
                int sum = mul + temp[i + j + 1];
                temp[i + j + 1] = sum % 10;
                temp[i + j] = temp[i + j] + (sum / 10);
            }
        }
        string s = "";
        bool leading = true;
        for (int p = 0; p < (int)temp.size(); p = p + 1) {
            if (leading == true && temp[p] == 0) {
                continue;
            }
            leading = false;
            s.push_back((char)(temp[p] + '0'));
        }
        if (s.length() == 0) {
            return BigInt("0");
        }
        return BigInt(s);
    }

    BigInt operator/(const BigInt& other) const {
        if (other.value == "0") {
            return BigInt("0");
        }
        if (this->isLess(this->value, other.value)) {
            return BigInt("0");
        }

        string res = "";
        string currentStr = "";
        for (int i = 0; i < (int)value.length(); i = i + 1) {
            currentStr.push_back(value[i]);
            while (currentStr.length() > 1 && currentStr[0] == '0') {
                currentStr.erase(0, 1);
            }

            int count = 0;
            BigInt currentBI(currentStr);
            while (!(currentBI < other)) {
                currentBI = currentBI - other;
                count = count + 1;
            }
            res.push_back((char)(count + '0'));
            currentStr = currentBI.value;
        }
        while (res.length() > 1 && res[0] == '0') {
            res.erase(0, 1);
        }
        return BigInt(res);
    }

    BigInt operator%(const BigInt& other) const {
        if (other.value == "0") return BigInt("0");
        BigInt q = (*this) / other;
        BigInt p = q * other;
        BigInt res = (*this) - p;
        return res;
    }
};

bool is_odd(const BigInt& n) {
    return (n.value.back() - '0') % 2 == 1;
}

BigInt half(const BigInt& n) {
    string res;
    int carry = 0;
    for (char ch : n.value) {
        int cur = carry * 10 + (ch - '0');
        res.push_back((cur / 2) + '0');
        carry = cur % 2;
    }
    int i = 0;
    while (i < (int)res.size() - 1 && res[i] == '0') ++i;
    return BigInt(res.substr(i));
}

BigInt mod_pow(const BigInt& base, const BigInt& exponent, const BigInt& modulus) {
    if (modulus.value == "0") return BigInt("0");
    BigInt result("1");
    BigInt b = base % modulus;
    BigInt e = exponent;

    while (e.value != "0") {
        if (is_odd(e)) {
            BigInt temp = result * b;
            result = temp % modulus;
        }
        BigInt temp = b * b;
        b = temp % modulus;
        e = half(e);
    }
    return result % modulus;
}


int main() {
    BigInt base("123456789");
    BigInt exponent("987654321");
    BigInt modulus("1000000007");

    BigInt res = mod_pow(base, exponent, modulus);
    cout << res.value << endl;
    return 0;
}