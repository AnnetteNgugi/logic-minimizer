#include <iostream>
#include <vector>
#include <string>

#include "input/input.h"
#include "minimizer/minimizer.h"
#include "output/output.h"

int main() {
    int n = getVariableCount();

    if (n < 2 || n > 4) {
        std::cout << "Error: only 2 to 4 variables are supported.\n";
        return 0;
    }

    std::vector<int> table = getTruthTable(n);
    std::vector<int> minterms = getMinterms(table);

    MinimizeResult result = minimize(n, minterms);

    printResult(result);

    return 0;
}
