#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
#include "../minimizer/minimizer.h"

// print K-map + simplified SOP + grouping explanation
void printResult(int n, const std::vector<int>& table, const MinimizeResult& result);

#endif
