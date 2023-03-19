//
// Created by wjc on 2023/3/19.
//
#include <iostream>
using namespace std;

class ObjectB
{
public:
    ObjectB(int objb) : objb_(objb)
    {
        cout<<"ObjectB()..."<<endl;
    }
    ~ObjectB()
    {
        cout<<"~ObjectB()..."<<endl;
    }
    int objb_;
};

class ObjectD
{
public:
    ObjectD(int objd) : objd_(objd)
    {
        cout<<"ObjectD()..."<<endl;
    }
    ~ObjectD()
    {
        cout<<"~ObjectD()..."<<endl;
    }
    int objd_;
};

class Base
{
public:
    Base(int b) : b_(b), objb_(222)
    {
        cout<<"Base()..."<<endl;
    }
    Base(const Base& other) : objb_(other.objb_), b_(other.b_)//, b_(other.b_)
    {
        cout<<"Base(const Base& other)..."<<endl;
    }
    ~Base()
    {
        cout<<"~Base()..."<<endl;
    }
    int b_;
    ObjectB objb_;
};


class Derived : public Base
{
public:
    Derived(int b, int d) : d_(d), Base(b), objd_(111) // 对继承来的基类成员的初始化（调用基类构造函数完成）派生类的构造函数需要给基类的构造函数传递参数
    {
        cout<<"Derived()..."<<endl;
    }
    Derived(const Derived& other) : d_(other.d_), objd_(other.objd_), Base(other)
    {

    }
    ~Derived()
    {
        cout<<"~Derived()..."<<endl;
    }

    int d_;
    ObjectD objd_;
};

int main(void)
{

    Derived d(100, 10);
    cout<<d.b_<<" "<<d.d_<<endl;

    Base b1(100);
    Base b2(b1);
    cout<<b2.b_<<endl;

    Derived d2(d);

    return 0;
}