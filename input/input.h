#ifndef INPUT_H
#define INPUT_H

#include <vector>

int getVariableCount();
std::vector<int> getTruthTable(int n);
std::vector<int> getMinterms(const std::vector<int>& table);

#endif
