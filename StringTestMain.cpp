//
// Created by wjc on 2023/3/18.
//
#include <string>
#include <iostream>
#include "StringUtil.h"
using namespace std;

int main(void)
{
    string s = "    abcd    ";
    StringUtil::LTrim(s);
    cout<<"["<<s<<"]"<<endl;

    s = "    abcd    ";
    StringUtil::RTrim(s);
    cout<<"["<<s<<"]"<<endl;

    s = "    abcd    ";
    StringUtil::Trim(s);
    cout<<"["<<s<<"]"<<endl;

    return 0;
}

void fun(char *str)
{
    cout<<str<<endl;
}

int main12345(void)
{
    string s1 = "abc";
    string s2("abcjjkkhuuguuu");
    cout<<s2<<endl;

    basic_string<char> s3("xxx");
    cout<<s3<<endl;

    string s4("abcdefg", 4);
    cout<<s4<<endl;

    string s5(s2, 2, 3);
    cout<<s5<<endl;

    string::iterator first = s2.begin() + 1;
    string::iterator last = s2.end();

    string s6(first, last);
    cout<<s6<<endl;
    cout<<s6.size()<<endl;
    cout<<s6.length()<<endl;
    cout<<s6.empty()<<endl;

    cout<<s2.substr(1, 2)<<endl;
    cout<<s2.substr(1, -1)<<endl;
    cout<<s2.substr(1, string::npos)<<endl;
    cout<<s2.substr(1)<<endl;

    string::size_type pos = s2.find("a", 1);
    if (pos == string::npos)
        cout<<"not found"<<endl;
    else
        cout<<"found pos="<<pos<<endl;

    pos = s2.rfind('k', 7);
    if (pos == string::npos)
        cout<<"not found"<<endl;
    else
        cout<<"found pos="<<pos<<endl;

    s2.replace(2, 2, "AAAAA");
    cout<<s2<<endl;

    s2.replace(s2.begin()+1, s1.begin()+4, "BBBBB");
    cout<<s2<<endl;

    s2.insert(2, "CCCC");
    cout<<s2<<endl;

    s2.append("6666");
    cout<<s2<<endl;

    s2.swap(s3);
    cout<<s2<<endl;
    cout<<s3<<endl;

    s1[1] = 'b';
    cout<<s1<<endl;

    const string ss2 = "xyz";
//    ss2[1] = 'Y';   // Error ss2[1] 返回的是const char&

    string ss3 = "111" + s2 + "222" + s1;
    cout<<ss3<<endl;

    //ss3.c_str()
    fun(const_cast<char*>(ss3.c_str()));

    string ss1(5, 'c');
    cout<<ss1<<endl;

    string strinfo = " ///*-----Hello World!.......------";
    string strset = "AVBCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string::size_type first1 = strinfo.find_first_of(strset);
    if (first1 == string::npos)
        cout<<"not find any characters"<<endl;
    string ::size_type last1 = strinfo.find_last_of(strset);
    if (last1 == string::npos)
        cout<<"not find any characters"<<endl;
    cout<<strinfo.substr(first1, last1 - first1 + 1)<<endl;



    return 0;
}