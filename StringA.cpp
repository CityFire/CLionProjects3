//
// Created by wjc on 2023/3/17.
//

#include "StringA.h"
#include <iostream>
//#include <string>
#include <cstring>
using namespace std;

StringA::StringA(char *str)
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

char *StringA::AllocAndCopy(char *str) {
    int len = strlen(str) + 1;
    char* tmp = new char[len];
    memset(tmp, 0, len);
    strcpy(tmp, str);
    return tmp;
}

StringA::~StringA()
{
    delete[] str_;
}

void StringA::Display() {
    cout<<str_<<endl;
}