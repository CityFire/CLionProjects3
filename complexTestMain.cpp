//
// Created by wjc on 2023/3/18.
//
#include "complex.h"
#include <iostream>
using namespace std;

int main0099(void)
{
    Complex c1(3, 4);
    Complex c2(4, 6);

//    c1.Add(c2);
//    c1.Display();

    Complex c3 = c1 + c2;   // 等价于c1.operator(c2) 或 operator+(c1, c2);
    //Complex c3 = c1.operator+(c2);
    c1.Display();
    c2.Display();
    c3.Display();

    return 0;
}