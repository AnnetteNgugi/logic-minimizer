#ifndef MINIMIZER_H
#define MINIMIZER_H

#include <vector>
#include <string>

struct GroupInfo {
    int groupSize;                 
    std::vector<int> minterms;     
    std::string term;              
};

struct MinimizeResult {
    std::vector<std::string> terms;  
    std::vector<GroupInfo> groups;   
};

MinimizeResult minimize(int n, const std::vector<int>& minterms);

#endif
