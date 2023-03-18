//
// Created by wjc on 2023/3/17.
//

#include "StringA.h"
#include <iostream>
//#include <string>
#include <cstring>
using namespace std;

char *StringA::AllocAndCopy(const char *str) {
    int len = strlen(str) + 1;
    char* tmp = new char[len];
    memset(tmp, 0, len);
    strcpy(tmp, str);
    return tmp;
}

StringA::StringA(const char *str)
{
    str_ = AllocAndCopy(str);
}

StringA::StringA(const StringA& other) {
    str_ = AllocAndCopy(other.str_);
}

StringA& StringA::operator=(const StringA &other) {
    if (this == &other)
        return *this;

    delete[] str_;
    str_ = AllocAndCopy(other.str_);
    return *this;
}

StringA& StringA::operator=(const char *str)
{
    delete[] str_;
    str_ = AllocAndCopy(str);
    return *this;
}

bool StringA::operator!() const
{
    return strlen(str_) != 0;
}

StringA::~StringA()
{
    delete[] str_;
}

void StringA::Display() const {
    cout<<str_<<endl;
}