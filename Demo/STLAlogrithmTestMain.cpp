//
// Created by wjc on 2023/3/28.
//
#include <map>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

void PrintFun(int n)
{
    cout<<n<<' ';
}

void Add3(int &n)
{
    n += 3;
}

class PrintObj
{
public:
    void operator()(int n)
    {
        cout<<n<<' ';
    }
};

class AddObj
{
public:
    AddObj(int number) : number_(number)
    {

    }
    void operator()(int& n)
    {
        n += number_;
    }
private:
    int number_;
};

class GreaterObj
{
public:
    GreaterObj(int number) : number_(number)
    {

    }
    bool operator()(int n)
    {
        return n > number_;
    }
private:
    int number_;
};

int FunObjectmain(void)
{
    int a[] = {1, 2, 3, 4, 5};
    vector<int> v(a, a+5);

    for_each(v.begin(), v.end(), PrintFun);
    cout<<endl;

    for_each(v.begin(), v.end(), PrintObj());
    cout<<endl;

    for_each(v.begin(), v.end(), Add3);
    for_each(v.begin(), v.end(), PrintFun);
    cout<<endl;

    for_each(v.begin(), v.end(), AddObj(5));
    for_each(v.begin(), v.end(), PrintFun);
    cout<<endl;

    cout<<count_if(a, a + 5, GreaterObj(3))<<endl;

    return 0;
}

struct MyGreater
{
    bool operator()(int left, int right)
    {
        return left > right;
    }
};

int mainmapTest(void)
{
    map<int, string, greater<int>> mapTest;
//    map<int, string, MyGreater> mapTest;
//    mapTest.insert(map<int, string>::value_type(1, "aaaa"));
    mapTest.insert(map<int, string>::value_type(1, "aaaa"));
    mapTest.insert(map<int, string>::value_type(3, "cccc"));
    mapTest.insert(map<int, string>::value_type(2, "bbbb"));

    for (map<int, string, greater<int>>::const_iterator it = mapTest.begin(); it != mapTest.end(); ++it)
//    for (map<int, string, MyGreater>::const_iterator it = mapTest.begin(); it != mapTest.end(); ++it)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }

    return 0;
}