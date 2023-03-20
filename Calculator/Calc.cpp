//
// Created by wjc on 2023/3/21.
//

#include "Calc.h"

unsigned int Calc::FindSymbol(const std::string &str) const
{
    return symTbl_.Find(str);
}

unsigned int Calc::AddSymbol(const std::string &str)
{
    return symTbl_.Add(str);
}