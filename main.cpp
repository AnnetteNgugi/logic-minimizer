#include <iostream>
#include "input/Input.h"
#include "minimizer/Minimizer.h"
#include "output/Printer.h"

using namespace std;

int main() {

    int n = input::getVariableCount();

    if (n < 2 || n > 4) {
        cout << "Invalid number of variables\n";
        return 0;
    }

    auto table = input::getTruthTable(n);
    auto minterms = input::getMinterms(table);

    auto result = minimizer::minimize(n, minterms);

    output::printResult(result);

    return 0;
}
