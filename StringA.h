//
// Created by wjc on 2023/3/17.
//

#ifndef DEMO_STRINGA_H
#define DEMO_STRINGA_H

#include <iostream>
using namespace std;

class StringA {
public:
    StringA(const char* str = "");
    StringA(const StringA& other);
    StringA& operator=(const StringA& other);
    StringA& operator=(const char* str);
    // !运算符重载
    bool operator!() const;
    // []运算符重载
    char& operator[](unsigned int index);
    const char& operator[](unsigned int index) const;
    //+运算符重载
    friend StringA operator+(const StringA& s1, const StringA& s2);
    //+=运算符重载
    StringA& operator+=(const StringA& other);
    //流<<和>>运算符重载
    friend ostream& operator<<(ostream& os, const StringA& str);
    friend istream& operator>>(istream& is, StringA& str);
    void Display() const;
    ~StringA(void);

private:
    StringA& Assign(const char* str);
    char* AllocAndCopy(const char* str);
    char* str_;
};


#endif //DEMO_STRINGA_H
