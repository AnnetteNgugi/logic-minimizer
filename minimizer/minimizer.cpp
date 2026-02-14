#include "minimizer.h"

std::string toBinary(int num, int n) {

    std::string s = "";

    for (int i = n - 1; i >= 0; i--)
        s += ((num >> i) & 1) ? '1' : '0';

    return s;
}

std::string termFromMinterm(int minterm, int n) {

    std::string vars = "ABCD";
    std::string binary = toBinary(minterm, n);
    std::string term = "";

    for (int i = 0; i < n; i++) {

        if (binary[i] == '1')
            term += vars[i];
        else {
            term += vars[i];
            term += '\'';
        }
    }

    return term;
}

std::vector<std::string> minimize(int n, const std::vector<int>& minterms) {

    std::vector<std::string> terms;

    for (int m : minterms)
        terms.push_back(termFromMinterm(m, n));

    return terms;
}
