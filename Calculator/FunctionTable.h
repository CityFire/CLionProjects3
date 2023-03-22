//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATOR_FUNCTIONTABLE_H
#define CALCULATOR_FUNCTIONTABLE_H

class SymbolTable;

typedef double (*PtrFun)(double);

class FunctionTable {
public:
    FunctionTable(SymbolTable& tbl);
    ~FunctionTable();

    void Init(SymbolTable& tbl);
    unsigned int Size() const
    {
        return size_;
    }
    PtrFun GetFunction(unsigned int id) const
    {
        return pFuns_[id];
    }

    unsigned int GetSize() const { return size_; }

private:
    PtrFun* pFuns_;
    unsigned int size_;
};


#endif //CALCULATOR_FUNCTIONTABLE_H
