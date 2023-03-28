//
// Created by wjc on 2023/3/28.
//
//#include <map>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

class Person
{
public:
    Person(const string& name) : name_(name) {}
    void Print() const { cout<<name_<<endl; }
    void PrintWithPrefix(string prefix) const { cout<<prefix<<name_<<endl; }
private:
    string name_;
};

void foo(const vector<Person>& v)
{
    for_each(v.begin(), v.end(), mem_fun_ref(&Person::Print));
    for_each(v.begin(), v.end(), bind2nd(mem_fun_ref(&Person::PrintWithPrefix), "person:"));
}

void foo2(const vector<Person*>& v)
{
    for_each(v.begin(), v.end(), mem_fun(&Person::Print));
    for_each(v.begin(), v.end(), bind2nd(mem_fun(&Person::PrintWithPrefix), "person:"));
}

bool is_odd(int n)
{
    return n % 2 == 0;
}

bool check(int elem)
{
    return elem < 3;
}

int mainAdapter(void)
{
    int a[] = {1, 2, 3, 4, 5};
    vector<int> v(a, a+5);

    cout<<count_if(v.begin(), v.end(), is_odd)<<endl;

    //计算奇数元素的个数
    //这里的bind2nd将二元函数对象modulus转换为一元函数对象。
    //bind2nd(op, value) (param)相当于op(param, value)
    cout<<count_if(v.begin(), v.end(), bind2nd(modulus<int>(), 2)) <<endl;

    //bind1st(op, value)(param)相当于op(value, param);
    cout<<count_if(v.begin(), v.end(), bind1st(less<int>(), 2))<<endl;


    // 针对成员函数的函数适配器
    vector<Person> v2;
    v2.push_back(Person("Tom"));
    v2.push_back(Person("Jerry"));
    foo(v2);

    vector<Person*> v3;
    v3.push_back(new Person("tom"));
    v3.push_back(new Person("jerry"));
    foo2(v3);


    vector<int>::iterator it;
//    it = find_if(v.begin(), v.end(), not1(check));
    it = find_if(v.begin(), v.end(), not1(ptr_fun(check)));
    if (it != v.end()) {
        cout<<*it<<endl;
    }

    // 针对一般函数的函数适配器
    char* a2[] = {"", "BBB", "CCC"};
    vector<char*> v4{a2, a2+2};

    vector<char*>::iterator it2;
    it2 = find_if(v4.begin(), v4.end(), bind2nd(ptr_fun(strcmp
    ), ""));
    if (it2 != v4.end())
        cout<<*it2<<endl;

    return 0;
}