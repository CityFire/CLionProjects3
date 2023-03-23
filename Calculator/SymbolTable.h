//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATOR_SYMBOLTABLE_H
#define CALCULATOR_SYMBOLTABLE_H

#include <map>
#include <string>
#include "Serializer.h"

class SymbolTable : Serializable {
public:
    enum { IDNOTFOUND = 0xffffffff };
    SymbolTable() : curId_(0) {}
    void Serialize(Serializer& out) const;
    void DeSerialize(DeSerializer& in);
    unsigned int Add(const std::string& str);
    unsigned int Find(const std::string& str) const;
    void Clear();
    std::string GetSymbolName(unsigned int id) const;

    unsigned int GetCurId() const { return curId_; }

private:
    std::map<const std::string, unsigned int> dictionary_;
    unsigned int curId_;
};


#endif //CALCULATOR_SYMBOLTABLE_H
