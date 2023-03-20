//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATOR_SYMBOLTABLE_H
#define CALCULATOR_SYMBOLTABLE_H

#include <map>
#include <string>

class SymbolTable {
    enum { IDNOTFOUND = 0xffffffff };
public:
    SymbolTable() : curId_(0) {}
    unsigned int Add(const std::string& str);
    unsigned int Find(const std::string& str) const;
    void Clear();
    std::string GetSymbolName(unsigned int id) const;
private:
    std::map<const std::string, unsigned int> dictionary_;
    unsigned int curId_;
};


#endif //CALCULATOR_SYMBOLTABLE_H