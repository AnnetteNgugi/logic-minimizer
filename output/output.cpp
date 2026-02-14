#include "output.h"
#include <iostream>
#include <algorithm>

void printResult(const MinimizeResult& result) {
    
    std::cout << "\nSimplified SOP:\nF = ";

    if (result.terms.size() == 1 && result.terms[0] == "0") {
        std::cout << "0\n\n";
        std::cout << "Grouping explanation:\nNo groups (output is always 0).\n";
        return;
    }

    for (int i = 0; i < (int)result.terms.size(); i++) {
        std::cout << result.terms[i];
        if (i != (int)result.terms.size() - 1) std::cout << " + ";
    }
    std::cout << "\n\n";

   
    std::cout << "Grouping explanation:\n";
    std::cout << "Each group corresponds to a K-map grouping (size 1,2,4,8,16).\n\n";

    
    std::vector<GroupInfo> groups = result.groups;
    std::sort(groups.begin(), groups.end(), [](const GroupInfo& a, const GroupInfo& b) {
        return a.groupSize > b.groupSize;
    });

    for (const GroupInfo& g : groups) {
        std::cout << "- Group size: " << g.groupSize << "\n";
        std::cout << "  Minterms covered: { ";
        for (int i = 0; i < (int)g.minterms.size(); i++) {
            std::cout << g.minterms[i];
            if (i != (int)g.minterms.size() - 1) std::cout << ", ";
        }
        std::cout << " }\n";
        std::cout << "  Term produced: " << g.term << "\n\n";
    }
}
