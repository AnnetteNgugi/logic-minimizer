#include "output.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

static std::vector<int> gray2bits() {
    // 2-bit Gray code sequence: 00, 01, 11, 10
    return {0, 1, 3, 2};
}

static std::string bitsToLabel(int value, int bits) {
    std::string s;
    for (int i = bits - 1; i >= 0; i--) {
        s.push_back(((value >> i) & 1) ? '1' : '0');
    }
    return s;
}

static void printKMap2(const std::vector<int>& table) {
    // 2 variables: rows = A (0,1), cols = B (0,1)
    std::cout << "K-MAP (2 variables)\n";
    std::cout << "      B\n";
    std::cout << "      0  1\n";
    for (int A = 0; A <= 1; A++) {
        std::cout << "A=" << A << "  ";
        for (int B = 0; B <= 1; B++) {
            int idx = (A << 1) | B; // AB
            std::cout << " " << table[idx] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

static void printKMap3(const std::vector<int>& table) {
    // 3 variables: rows = A (0,1), cols = BC in Gray order
    std::vector<int> col = gray2bits(); // BC
    std::cout << "K-MAP (3 variables)\n";
    std::cout << "        BC\n";
    std::cout << "       ";
    for (int v : col) std::cout << " " << bitsToLabel(v, 2) << " ";
    std::cout << "\n";

    for (int A = 0; A <= 1; A++) {
        std::cout << "A=" << A << "   ";
        for (int bc : col) {
            int B = (bc >> 1) & 1;
            int C = (bc >> 0) & 1;
            int idx = (A << 2) | (B << 1) | C; // ABC
            std::cout << "  " << table[idx] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

static void printKMap4(const std::vector<int>& table) {
    // 4 variables: rows = AB in Gray order, cols = CD in Gray order
    std::vector<int> row = gray2bits(); // AB
    std::vector<int> col = gray2bits(); // CD

    std::cout << "K-MAP (4 variables)\n";
    std::cout << "           CD\n";
    std::cout << "        ";
    for (int v : col) std::cout << " " << bitsToLabel(v, 2) << " ";
    std::cout << "\n";

    for (int ab : row) {
        std::cout << "AB=" << bitsToLabel(ab, 2) << "  ";
        int A = (ab >> 1) & 1;
        int B = (ab >> 0) & 1;

        for (int cd : col) {
            int C = (cd >> 1) & 1;
            int D = (cd >> 0) & 1;
            int idx = (A << 3) | (B << 2) | (C << 1) | D; // ABCD
            std::cout << "  " << table[idx] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

static void printKMap(int n, const std::vector<int>& table) {
    if (n == 2) printKMap2(table);
    else if (n == 3) printKMap3(table);
    else if (n == 4) printKMap4(table);
}

void printResult(int n, const std::vector<int>& table, const MinimizeResult& result) {
    // 1) K-map display
    printKMap(n, table);

    // 2) Print simplified SOP
    std::cout << "Simplified SOP:\nF = ";

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

    // 3) Grouping explanation
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
