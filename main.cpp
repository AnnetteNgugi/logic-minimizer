#include <iostream>
#include <vector>
#include "input/Input.hpp"
#include "minimizer/Minimizer.hpp"
#include "output/Printer.hpp"

int main() {

    int n = getVariableCount();

    if (n < 2 || n > 4) {
        std::cout << "Invalid number of variables\n";
        return 0;
    }

    std::vector<int> table = getTruthTable(n);
    std::vector<int> minterms = getMinterms(table);

    std::vector<std::string> result = minimize(n, minterms);

    printResult(result);

    return 0;
}
