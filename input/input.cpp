#include "input.h"
#include <iostream>

int getVariableCount() {
    int n;
    std::cout << "Enter number of variables (2-4): ";
    std::cin >> n;
    return n;
}

std::vector<int> getTruthTable(int n) {
    int size = 1 << n;
    std::vector<int> table(size);

    std::cout << "Enter " << size << " output values (0/1) for minterms 0.." << (size - 1) << ":\n";
    std::cout << "Example for n=2: enter 4 values for rows 00,01,10,11\n\n";

    for (int i = 0; i < size; i++) {
        std::cin >> table[i];
    }

    return table;
}

std::vector<int> getMinterms(const std::vector<int>& table) {
    std::vector<int> minterms;
    for (int i = 0; i < (int)table.size(); i++) {
        if (table[i] == 1) {
            minterms.push_back(i);
        }
    }
    return minterms;
}
