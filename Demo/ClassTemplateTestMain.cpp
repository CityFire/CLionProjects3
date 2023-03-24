//
// Created by wjc on 2023/3/23.
//
#include <iostream>
#include <vector>
using namespace std;
#include "Stack.h"
#include "Stack2.h"
#include "Stack3.h"

template <typename T>
class MyClass2
{
private:
    typename T::SubType* ptr_;
};

class Test2
{
public:
    typedef int SubType;
};

template <typename T>
class MyClass
{
private:
    T value;
public:
    // 成员模板
    template <class X>
    void Assign(const MyClass<X>& x)
    {
        value = x.GetValue();
    }
    T GetValue() const { return value; }
};

int mainzzzzzzzzza(void)
{
    MyClass<double> d;
    MyClass<int> i;

    d.Assign(d);  // OK
    d.Assign(i);  // OK

    MyClass2<Test2> mc;


    return 0;
}

/*
template <typename T>
class MyClass
{
private:
    T value;
public:
    void Assign(const MyClass<T>& x)
    {
        value = x.value;
    }
};

int mainClassTemp(void)
{
    MyClass<double> d;
    MyClass<int> i;

    d.Assign(d);  // OK
    d.Assign(i);  // Error

    return 0;
}
 */

int mainStack3(void)
{
    Stack3<int, vector<int> > s;
    s.Push(1);
    s.Push(2);
    s.Push(3);

    while (!s.Empty())
    {
        cout<<s.Top()<<endl;
        s.Pop();
    }

    return 0;
}

int mainStack2(void)
{
    Stack2<int, 3> s;
    s.Push(1);
    s.Push(2);
    s.Push(3);

    while (!s.Empty())
    {
        cout<<s.Top()<<endl;
        s.Pop();
    }

    return 0;
}

int mainClassTemplate(void)
{
    Stack<int> s(10);
    s.Push(1);
    s.Push(2);
    s.Push(3);

    while (!s.Empty())
    {
        cout<<s.Top()<<endl;
        s.Pop();
    }

    return 0;
}