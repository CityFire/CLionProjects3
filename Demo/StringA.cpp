//
// Created by wjc on 2023/3/17.
//

#include "StringA.h"
//#include <string>
#include <cstring>
//#include <iostream>
//using namespace std;

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

    return Assign(other.str_);
}

StringA& StringA::operator=(const char *str)
{
    return Assign(str);
}

bool StringA::operator!() const
{
    return strlen(str_) != 0;
}

char& StringA::operator[](unsigned int index)
{
    return str_[index];
    //non const 版本调用 const版本

    return const_cast<char&>(static_cast<const StringA&>(*this)[index]);
}

const char& StringA::operator[](unsigned int index) const
{
    return str_[index];
}

//+运算符重载
StringA operator+(const StringA& s1, const StringA& s2)
{
//    int len = strlen(s1.str_) + strlen(s2.str_) + 1;
//    char* newstr = new char[len];
//    memset(newstr, 0, len);
//    strcpy(newstr, s1.str_);
//    strcat(newstr, s2.str_);
//
//    StringA tmp(newstr);
//    delete newstr;
    StringA str = s1;
    str += s2;
    return str;
}

StringA &StringA::operator+=(const StringA &other)
{
    int len = strlen(str_) + strlen(other.str_) + 1;
    char* newstr = new char[len];
    memset(newstr, 0, len);
    strcpy(newstr, str_);
    strcat(newstr, other.str_);

    delete str_;
    str_ = newstr;
    return *this;
}

ostream& operator<<(ostream& os, const StringA& str)
{
    os<<str.str_;
    return os;
}

istream& operator>>(istream& is, StringA& str)
{
    char tmp[1024];
    cin>>tmp;
    str = tmp;
    return is;
}

StringA &StringA::Assign(const char *str)
{
    delete[] str_;
    str_ = AllocAndCopy(str);
    return *this;
}

StringA::~StringA()
{
    delete[] str_;
}

void StringA::Display() const {
    cout<<str_<<endl;
}