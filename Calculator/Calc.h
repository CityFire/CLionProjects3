//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATOR_CALC_H
#define CALCULATOR_CALC_H

#include "SymbolTable.h"
#include "Storage.h"
#include <string>

class Parser;

class Calc {
    friend class Parser;
public:
    Calc() : storage_(symTbl_) {}
private:
    Storage& GetStorage()
    {
        return storage_;
    }
    unsigned int AddSymbol(const std::string& str);
    unsigned int FindSymbol(const std::string& str) const;
    SymbolTable symTbl_;
    Storage storage_;
};


#endif //CALCULATOR_CALC_H
