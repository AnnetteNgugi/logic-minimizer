#include "output.h"
#include <iostream>

void printResult(const std::vector<std::string>& terms) {

    std::cout << "\nSimplified SOP:\nF = ";

    for (size_t i = 0; i < terms.size(); i++) {

        std::cout << terms[i];

        if (i != terms.size() - 1)
            std::cout << " + ";
    }

    std::cout << std::endl;
}
