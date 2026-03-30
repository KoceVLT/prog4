#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

class BigInt {
public:
    string value;

    BigInt() : value("0") {}

    BigInt(string s) {
        int i = 0;
        while (i < (int)s.length() - 1 && s[i] == '0') {
            i++;
        }
        value = s.substr(i);
    }

    bool isLess(const string &n1, const string &n2) const {
        if (n1.length() != n2.length())
            return n1.length() < n2.length();
        return n1 < n2;
    }

    bool operator<(const BigInt &other) const {
        return isLess(value, other.value);
    }

    bool operator==(const BigInt &other) const {
        return value == other.value;
    }

    bool operator!=(const BigInt &other) const {
        return !(*this == other);
    }

    bool operator>(const BigInt &other) const {
        return other < *this;
    }

    bool operator<=(const BigInt &other) const {
        return !(other < *this);
    }

    bool operator>=(const BigInt &other) const {
        return !(*this < other);
    }

    BigInt operator+(const BigInt &other) const {
        string s1 = value, s2 = other.value;
        string res;
        int i = (int)s1.length() - 1, j = (int)s2.length() - 1, carry = 0;

        while (i >= 0 || j >= 0 || carry) {
            int v1 = (i >= 0) ? s1[i] - '0' : 0;
            int v2 = (j >= 0) ? s2[j] - '0' : 0;
            int sum = v1 + v2 + carry;
            res.push_back(char((sum % 10) + '0'));
            carry = sum / 10;
            i--; j--;
        }
        reverse(res.begin(), res.end());

        size_t pos = 0;
        while (pos + 1 < res.length() && res[pos] == '0') pos++;
        res = res.substr(pos);
        return BigInt(res);
    }

    BigInt operator-(const BigInt &other) const {
        if (*this < other) return BigInt("0");

        string s1 = value, s2 = other.value;
        string res;
        int i = (int)s1.length() - 1, j = (int)s2.length() - 1, borrow = 0;

        while (i >= 0) {
            int v1 = s1[i] - '0';
            int v2 = (j >= 0) ? s2[j] - '0' : 0;
            v2 += borrow;

            if (v1 < v2) {
                v1 += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            res.push_back(char((v1 - v2) + '0'));
            i--; j--;
        }
        while (res.length() > 1 && res.back() == '0')
            res.pop_back();
        reverse(res.begin(), res.end());
        return BigInt(res);
    }

    BigInt operator*(const BigInt &other) const {
        int n = (int)value.length(), m = (int)other.value.length();
        vector<int> temp(n + m, 0);

        for (int i = n - 1; i >= 0; --i) {
            for (int j = m - 1; j >= 0; --j) {
                int mul = (value[i] - '0') * (other.value[j] - '0');
                int sum = mul + temp[i + j + 1];
                temp[i + j + 1] = sum % 10;
                temp[i + j] += sum / 10;
            }
        }

        string res;
        bool leading = true;
        for (int p = 0; p < (int)temp.size(); ++p) {
            if (leading && temp[p] == 0) continue;
            leading = false;
            res.push_back(char(temp[p] + '0'));
        }
        return res.empty() ? BigInt("0") : BigInt(res);
    }

    BigInt operator/(const BigInt &other) const {
        if (other.value == "0") return BigInt("0");
        if (*this < other) return BigInt("0");

        string res, current;
        for (int i = 0; i < (int)value.length(); ++i) {
            current.push_back(value[i]);
            while (current.length() > 1 && current[0] == '0')
                current.erase(0, 1);

            int cnt = 0;
            BigInt curBI(current);
            while (!(curBI < other)) {
                curBI = curBI - other;
                cnt++;
            }
            res.push_back(char(cnt + '0'));
            current = curBI.value;
        }
        while (res.length() > 1 && res[0] == '0')
            res.erase(0, 1);
        return BigInt(res);
    }

    BigInt operator%(const BigInt &other) const {
        if (other.value == "0") return BigInt("0");
        BigInt q = *this / other;
        BigInt p = q * other;
        return *this - p;
    }
};

struct Entry {
    BigInt composite;
    BigInt prime;
};

struct CompareEntry {
    bool operator()(const Entry& a, const Entry& b) const {
        return a.composite > b.composite;
    }
};

int main() {
    int PRIME_COUNT = 100;
    vector<BigInt> primes;
    primes.push_back(BigInt("2"));

    priority_queue<Entry, vector<Entry>, CompareEntry> pq;
    Entry e;
    e.composite = BigInt("4");
    e.prime = BigInt("2");
    pq.push(e);

    BigInt n("3");
    while (primes.size() < PRIME_COUNT) {
        while (!pq.empty() && pq.top().composite < n) {
            Entry top = pq.top(); pq.pop();
            Entry newEntry;
            newEntry.composite = top.composite + top.prime;
            newEntry.prime = top.prime;
            pq.push(newEntry);
        }

        if (!pq.empty() && pq.top().composite == n) {
            Entry top = pq.top(); pq.pop();
            Entry newEntry;
            newEntry.composite = top.composite + top.prime;
            newEntry.prime = top.prime;
            pq.push(newEntry);
            n = n + BigInt("1");
        } else {
            primes.push_back(n);
            Entry newEntry;
            newEntry.composite = n * n;
            newEntry.prime = n;
            pq.push(newEntry);
            n = n + BigInt("1");
        }
    }

    cout << "Prvi " << PRIME_COUNT << " prosti:\n";
    for (size_t i = 0; i < primes.size(); ++i) {
        cout << primes[i].value;
        if ((i + 1) % 10 == 0) cout << '\n';
        else if (i + 1 != primes.size()) cout << ' ';
    }


    return 0;
}