//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATOR_STORAGE_H
#define CALCULATOR_STORAGE_H

#include <vector>

class SymbolTable;

class Storage {
public:
    Storage(SymbolTable& tbl);
    void Clear();
    bool IsInit(unsigned int id) const;
    void AddConstants(SymbolTable& tbl);
    double GetValue(unsigned int id) const;
    void SetValue(unsigned int id, double val);
    void AddValue(unsigned int id, double val);
private:
    std::vector<double> cells_;
    std::vector<bool> inits_;
};


#endif //CALCULATOR_STORAGE_H
