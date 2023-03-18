//
// Created by wjc on 2023/3/18.
//
#include "Integer.h"
#include "StringA.h"
#include <iostream>
using namespace std;

int main(void)
{
    Integer n(100);
    n.Display();

    Integer n2 = ++n;
    n.Display();
    n2.Display();

    Integer n3 = n++;
    n.Display();
    n3.Display();


    StringA s1("AAAA");
    StringA s2(s1);   // 调用默认的拷贝构造函数
    // 系统提供的默认拷贝构造函数实施的是浅拷贝 s2.str_ = s1.str_  导致释放两次

    StringA s3;
    s3 = s1;
    s3.Display();

    s3 = "xxxx";
    s3.Display();

    StringA s4;
    bool notempty;
    notempty = !s4;
    cout<<notempty<<endl;

    s4 = "aaaa";
    notempty = !s4;
    cout<<notempty<<endl;

    StringA ss1("abcdefg");

    char ch = ss1[2];
    cout<<ch<<endl;

    ss1[2] = 'A';
    ss1.Display();

    const StringA ss2("background");
    ch = ss2[2];
    //ss2[2] = 'M';
    ss2.Display();

    StringA ss3 = "xxx";
    StringA ss4 = "yyy";

    StringA ss5 = ss3 + ss4;
    ss5.Display();

//    StringA ss6 = "bbbb" + "aaa" + ss5 + "rrdfddd";
    StringA ss6 = "aaa" + ss5 + "rrdfddd";
    ss6.Display();

    ss3 += ss4;
    ss3.Display();

    return 0;
}